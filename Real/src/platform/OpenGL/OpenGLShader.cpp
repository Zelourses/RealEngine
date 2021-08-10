#include "repch.h"
#include "OpenGLShader.h"

#include "repch.h"

#include <glad/glad.h>
#include <fstream>

#include <glm/gtc/type_ptr.hpp>

namespace Real {


	OpenGLShader::OpenGLShader(const std::string& filePath) {
		auto shaderSrc = readFile(filePath);

		auto shaders = preProcess(shaderSrc);
		compile(shaders);
	}
	

	//pixelSrc is in OpenGL terms fragmentSrc. Yep, fragment shader.
	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& pixelSrc) {
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = pixelSrc;

		compile(sources);
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(rendererId);
	}


	std::string OpenGLShader::readFile(const std::string& filePath) {

		std::string result;

		std::ifstream input(filePath, std::ios::in, std::ios::binary);
		if (input) {
			input.seekg(0, std::ios::end);
			result.resize(input.tellg());

			input.seekg(0, std::ios::beg);
			input.read(&result[0], result.size());
			input.close();
		}
		else {
			RE_CORE_ERROR("Could not open file '{0}'", filePath);
		}

		return result;
	}

	static GLenum shaderTypeFromString(const std::string& type) {
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "pixel") return GL_FRAGMENT_SHADER;

		RE_CORE_ASSERT(false, "Unknown shader type");
		return 0;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::preProcess(const std::string& shaderSrc) {
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = shaderSrc.find(typeToken, 0);

		while(pos != std::string::npos) {
			size_t endOfLine = shaderSrc.find_first_of("\r\n", pos); //FIXME: windows-specific thing
			RE_CORE_ASSERT(endOfLine != std::string::npos, "Syntax error");
			
			size_t begin = pos + typeTokenLength + 1; //FIXME: not trimming whitespaces
			std::string type = shaderSrc.substr(begin, endOfLine - begin);
			RE_CORE_ASSERT(shaderTypeFromString(type),
				"Invalid shader type specified");

			size_t nextLinePosition = shaderSrc.find_first_not_of("\r\n", endOfLine); //FIXME: windows-specific
			pos = shaderSrc.find(typeToken, nextLinePosition);
			shaderSources[shaderTypeFromString(type)] =
				shaderSrc.substr(nextLinePosition,
					pos - (nextLinePosition == std::string::npos ? shaderSrc.size() - 1
						: nextLinePosition));
		}

		return shaderSources;
	}
	//modified version from official OpenGL wiki: https://www.khronos.org/opengl/wiki/Shader_Compilation#Example
	void OpenGLShader::compile(const std::unordered_map<GLenum, std::string> shaderSrc) {

		auto program = glCreateProgram();

		std::vector<GLenum> glShaderIds(shaderSrc.size());
		
		for (auto[shaderType, shaderString] : shaderSrc) {
			
			// Create an empty shader handle
			GLuint shader = glCreateShader(shaderType);

			const GLchar* source = shaderString.c_str();
			glShaderSource(shader, 1, &source, 0);

			// Compile the shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				// Use the infoLog as you see fit.
				RE_CORE_ERROR("{0}", infoLog.data());
				RE_CORE_ASSERT(false, "Shader compilation failure");
				break;
			}
			glAttachShader(program, shader);
			glShaderIds.push_back(shader);
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for(auto id : glShaderIds) {
				glDeleteShader(id);
			}
			// Use the infoLog as you see fit.

			RE_CORE_ERROR("{0}", infoLog.data());
			RE_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShaderIds) {
			glDetachShader(program,id);
		}

		rendererId = program;
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
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}
