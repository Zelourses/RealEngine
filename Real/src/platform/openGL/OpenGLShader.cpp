#include "repch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <utility>
#include <filesystem>

#include <shaderc/shaderc.hpp>

#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Utils {

	static std::string_view getCacheDirName() {
		//TODO: make sure that assets dir is existing
		return "assets/cache/shader/opengl";
	}

	static void createCacheDirIfNeeded() {
		auto&& cacheDir = getCacheDirName();
		if (!std::filesystem::exists(cacheDir)) {
			std::filesystem::create_directories(cacheDir);
		}
	}

	static GLenum shaderTypeFromString(const std::string& type) {
		RE_PROFILE_FUNCTION();
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment") return GL_FRAGMENT_SHADER;

		RE_CORE_ASSERT(false, "Unknown shader type");
		return 0;
	}

	static std::string_view glShaderStageCachedGLFileExtension(GLenum stage) {
		switch (stage) {
			case GL_VERTEX_SHADER: return ".gl_cached.vert";
			case GL_FRAGMENT_SHADER: return ".gl_cached.frag";
		}

		RE_CORE_ASSERT(false);
		return "";
	}

	static std::string_view glShaderStageCachedVKFileExtension(GLenum stage) {
		switch (stage) {
			case GL_VERTEX_SHADER: return ".vk_cached.vert";
			case GL_FRAGMENT_SHADER: return ".vk_cached.frag";
		}

		RE_CORE_ASSERT(false);
		return "";
	}

	static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage) {
		switch (stage) {
			case GL_VERTEX_SHADER: return shaderc_glsl_vertex_shader;
			case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
		}
		RE_CORE_ASSERT(false, "Unknown type of shader");
		return static_cast<shaderc_shader_kind>(0);
	}

	static std::string_view GLShaderStageToString(GLenum stage) {
		switch (stage) {
			case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
			case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
		}
		RE_CORE_ASSERT(false);
		return "";
	}

	static void turnOptimizationOptions(shaderc::CompileOptions& opt, bool optimize) {
		if (optimize) {
			opt.SetOptimizationLevel(shaderc_optimization_level_performance);
		} else {
			opt.SetOptimizationLevel(shaderc_optimization_level_zero);
			opt.SetGenerateDebugInfo();
		}
	}

	// Here I am trying to invent a wheel for not-so-bad and not-so-slow file reading
	// 1)It's actually very dangerous to read files by using tellg()
	// 2)Actually you can't get actual size of a file until you read it.
	// see: https://stackoverflow.com/questions/22984956/tellg-function-give-wrong-size-of-file/22986486#22986486
	//
	// TODO: create different functions for file reading/writing, and test the most efficient ways to do that
	// see: https://stackoverflow.com/questions/2912520/read-file-contents-into-a-string-in-c/2912614

	/**
	* This templated function is needed when we want to read something that is not 'char' or 'wchar', 
	*  like spirv is asking us to get vector of unsigned
	*/
	template <typename ReadType>
	static std::vector<ReadType> readFile(std::string_view filepath) {
		RE_PROFILE_FUNCTION();

		auto size = std::filesystem::file_size(filepath);

		std::ifstream f(filepath.data(), std::ios::in | std::ios::binary);
		std::vector<ReadType>		  buffer;
		buffer.resize(size / sizeof(ReadType));
		f.read(reinterpret_cast<char*>(buffer.data()), size);

		return buffer;
	}

	static std::string readFile(std::string_view filepath) {
		RE_PROFILE_FUNCTION();

		auto size = std::filesystem::file_size(filepath);

		std::ifstream f(filepath.data(), std::ios::in | std::ios::binary);
		std::string	  buffer;
		buffer.resize(size);
		f.read(buffer.data(), size);

		return buffer;
	}
}

namespace Real {


	OpenGLShader::OpenGLShader(const std::string& filepath)
		: filepath(filepath), rendererId(-1) {
		RE_PROFILE_FUNCTION();
		if (!std::filesystem::exists(filepath)) {
			RE_CORE_ERROR("Shader on path {0} does not exists", filepath);
			return;
		}

		Utils::createCacheDirIfNeeded();
		auto shaderSrc = Utils::readFile(filepath);

		auto shaders = preProcess(shaderSrc);

		{
			Timer t;
			compileOrGetVulkanBinaries(shaders);
			compileOrGetOpenGLBinaries();
			createProgram();
			RE_CORE_INFO("Shader creation took {0} ms", t.elapsedMillis());
		}

		std::filesystem::path path(filepath);
		name = path.filename().string();
	}


	OpenGLShader::OpenGLShader(
		const std::string& name,
		const std::string& vertexSrc,
		const std::string& fragmentSrc)
		: rendererId(-1), name(name) { // -1 in rendererId is a sign of an error
		RE_PROFILE_FUNCTION();
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER]	= vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;

		{
			Timer t;
			compileOrGetVulkanBinaries(sources);
			compileOrGetOpenGLBinaries();
			createProgram();
			RE_CORE_INFO("Shader createion took {0} ms", t.elapsedMillis());
		}

		RE_CORE_ASSERT(rendererId != static_cast<unsigned int>(-1), "Error while initializing shader")

		//For OpenGL debugging programs
		glObjectLabel(GL_SHADER, rendererId, static_cast<GLsizei>(name.length()), name.c_str());
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(rendererId);
	}


	std::unordered_map<GLenum, std::string> OpenGLShader::preProcess(const std::string& shaderSrc) {
		RE_PROFILE_FUNCTION();
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken		= "#type";
		size_t		typeTokenLength = strlen(typeToken);
		size_t		pos				= shaderSrc.find(typeToken, 0);

		while (pos != std::string::npos) {
			size_t endOfLine = shaderSrc.find_first_of("\r\n", pos); //FIXME: windows-specific thing
			RE_CORE_ASSERT(endOfLine != std::string::npos, "Syntax error");

			size_t		begin = pos + typeTokenLength + 1; //FIXME: not trimming whitespaces
			std::string type  = shaderSrc.substr(begin, endOfLine - begin);
			RE_CORE_ASSERT(Utils::shaderTypeFromString(type),
						   "Invalid shader type specified");

			size_t nextLinePosition = shaderSrc.find_first_not_of("\r\n", endOfLine); //FIXME: windows-specific

			pos = shaderSrc.find(typeToken, nextLinePosition);

			shaderSources[Utils::shaderTypeFromString(type)] = shaderSrc.substr(nextLinePosition,
																				pos - (nextLinePosition == std::string::npos ? shaderSrc.size() - 1 : nextLinePosition));
		}

		return shaderSources;
	}

	//This functions do not use profiling for getting more precise results in their caller functons
	void OpenGLShader::compileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& srcs) {
		shaderc::Compiler		compiler;
		shaderc::CompileOptions opt;
		opt.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);

		constexpr bool optimize = false;
		Utils::turnOptimizationOptions(opt, optimize);

		auto cacheDir = std::filesystem::path(Utils::getCacheDirName());

		for (auto&& [stage, shaderString]: srcs) {
			auto cachedFilepath = cacheDir / (filepath.filename().string().append(Utils::glShaderStageCachedVKFileExtension(stage)));

			if (std::filesystem::exists(cachedFilepath) && std::filesystem::is_regular_file(cachedFilepath)) {
				vulkanSPIRV[stage] = Utils::readFile<unsigned>(cachedFilepath.string());
			} else {
				auto compileModule = compiler.CompileGlslToSpv(shaderString, Utils::GLShaderStageToShaderC(stage), filepath.string().c_str(), opt);
				if (compileModule.GetCompilationStatus() != shaderc_compilation_status_success) {
					RE_CORE_ERROR("Shader compilation to spirv error: {0}", compileModule.GetErrorMessage());
					//a little bit rough, but on that stage is ok
					RE_CORE_ASSERT(false);
				}

				vulkanSPIRV[stage] = {compileModule.begin(), compileModule.end()};

				std::ofstream out(cachedFilepath, std::ios::out | std::ios::binary);

				if (out) {
					auto&& data = vulkanSPIRV[stage];
					out.write(reinterpret_cast<const char*>(data.data()), data.size() * sizeof(unsigned));
				} else {
					RE_CORE_ERROR("Can't create cached shader file: {0}", cachedFilepath);
				}
			}
		}
		for (auto&& [stage, data]: vulkanSPIRV)
			reflect(stage, data);
	}

	void OpenGLShader::compileOrGetOpenGLBinaries() {
		shaderc::Compiler		compiler;
		shaderc::CompileOptions opt;
		opt.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		Utils::turnOptimizationOptions(opt, false);

		openGLSPIRV.clear();
		openGLSourceCode.clear();
		auto cacheDir = std::filesystem::path(Utils::getCacheDirName());

		for (const auto& [stage, spirv]: vulkanSPIRV) {
			auto cachedFilepath = cacheDir / (filepath.filename().string().append(Utils::glShaderStageCachedGLFileExtension(stage)));

			if (std::filesystem::exists(cachedFilepath) && std::filesystem::is_regular_file(cachedFilepath)) {
				openGLSPIRV[stage] = Utils::readFile<unsigned>(cachedFilepath.string());
			} else {
				auto glslCompiler		= spirv_cross::CompilerGLSL(spirv);
				openGLSourceCode[stage] = glslCompiler.compile();

				auto&& compileModule = compiler.CompileGlslToSpv(openGLSourceCode[stage], Utils::GLShaderStageToShaderC(stage), filepath.string().c_str());
				if (compileModule.GetCompilationStatus() != shaderc_compilation_status_success) {
					RE_CORE_ERROR("Shader compilation to spirv error: {0}", compileModule.GetErrorMessage());
					//a little bit rough, but on that stage is ok
					RE_CORE_ASSERT(false);
				}

				openGLSPIRV[stage] = {compileModule.cbegin(),
									  compileModule.cend()};

				std::ofstream out(cachedFilepath, std::ios::out | std::ios::binary);

				if (out) {
					auto&& data = vulkanSPIRV[stage];
					out.write(reinterpret_cast<const char*>(data.data()), data.size() * sizeof(unsigned));
				} else {
					RE_CORE_ERROR("Can't create cached shader file: {0}", cachedFilepath);
				}
			}
		}
	}
	void OpenGLShader::createProgram() {
		auto program = glCreateProgram();

		std::vector<GLuint> shaderIds;
		shaderIds.reserve(openGLSPIRV.size());
		for (auto&& [stage, spirv]: openGLSPIRV) {
			auto shaderId = shaderIds.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shaderId, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), static_cast<GLsizei>(spirv.size() * sizeof(unsigned)));
			glSpecializeShader(shaderId, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderId);
		}

		glLinkProgram(program);

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			//Here actually must be something like vector<GLchar>, but I want to use string
			std::string infoLog;
			infoLog.reserve(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());

			glDeleteProgram(program);

			for (auto&& id: shaderIds) {
				glDeleteShader(id);
			}

			RE_CORE_ERROR("Shader linking failed in file ({0}):\n{1}", filepath, infoLog.data());
		}

		for (auto&& id: shaderIds) {
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		rendererId = program;
	}
	void OpenGLShader::reflect(GLenum stage, const std::vector<unsigned>& shaderData) {
		spirv_cross::Compiler compiler(shaderData);
		const auto			  resources = compiler.get_shader_resources();


		RE_CORE_TRACE("OpenGLShader::reflect -- {0} {1}\n"
					  "\t{2} uniform buffers\n"
					  "\t{3} resources\n"
					  "Uniform buffers:",
					  Utils::GLShaderStageToString(stage),
					  filepath,
					  resources.uniform_buffers.size(),
					  resources.sampled_images.size());

		for (const auto& resource: resources.uniform_buffers) {
			auto&& bufferType  = compiler.get_type(resource.base_type_id);
			auto&& buffersize  = compiler.get_declared_struct_size(bufferType);
			auto&& binding	   = compiler.get_decoration(resource.id, spv::DecorationBinding);
			auto&& memberCount = bufferType.member_types.size();

			RE_CORE_TRACE("\t{0}"
						  "\t size = {1}"
						  "\t binding = {2}"
						  "\t members = {3}",
						  resource.name,
						  buffersize,
						  binding,
						  memberCount);
		}
	}

	void OpenGLShader::setInt(const std::string& name, int value) {
		RE_PROFILE_FUNCTION();
		uploadUniformInt(name, value);
	}

	void OpenGLShader::setIntArray(const std::string& name, int* values, unsigned count) {
		RE_PROFILE_FUNCTION();
		uploadUniformIntArray(name, values, count);
	}

	void OpenGLShader::setFloat3(const std::string& name, const glm::vec3& value) {
		RE_PROFILE_FUNCTION();
		uploadUniformFloat3(name, value);
	}

	void OpenGLShader::setFloat4(const std::string& name, const glm::vec4& value) {
		RE_PROFILE_FUNCTION();
		uploadUniformFloat4(name, value);
	}

	void OpenGLShader::setMat4(const std::string& name, const glm::mat4& value) {
		RE_PROFILE_FUNCTION();
		uploadUniformMat4(name, value);
	}

	void OpenGLShader::bind() const {
		glUseProgram(rendererId);
	}
	void OpenGLShader::unbind() const {
		glUseProgram(0);
	}

	void OpenGLShader::uploadUniformInt(const std::string& name, int value) const {
		GLint location = glGetUniformLocation(rendererId, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::uploadUniformIntArray(const std::string& name, int* values, unsigned count) const {
		GLint location = glGetUniformLocation(rendererId, name.c_str());
		glUniform1iv(location, count, values);
	}


	void OpenGLShader::uploadUniformFloat(const std::string& name, float value) const {
		GLint location = glGetUniformLocation(rendererId, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::uploadUniformFloat2(const std::string& name, const glm::vec2& vector) const {
		GLint location = glGetUniformLocation(rendererId, name.c_str());
		glUniform2f(location, vector.x, vector.y);
	}

	void OpenGLShader::uploadUniformFloat3(const std::string& name, const glm::vec3& vector) const {
		GLint location = glGetUniformLocation(rendererId, name.c_str());
		glUniform3f(location, vector.x, vector.y, vector.z);
	}

	void OpenGLShader::uploadUniformFloat4(const std::string& name, const glm::vec4& vector) const {
		GLint location = glGetUniformLocation(rendererId, name.c_str());
		glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
	}


	void OpenGLShader::uploadUniformMat3(const std::string& name, const glm::mat3& matrix) const {
		GLint location = glGetUniformLocation(rendererId, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix) const {
		//glUseProgram(rendererId);
		GLint location = glGetUniformLocation(rendererId, name.c_str());
		RE_CORE_ASSERT(location >= 0, "Something is wrong in initialization of uniform");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}
