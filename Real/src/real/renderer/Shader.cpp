#include "repch.h"
#include "Shader.h"

#include "Renderer.h"
#include "platform/OpenGL/OpenGLShader.h"

namespace Real {
	Ref<Shader> Shader::create(const std::string& filePath) {
		switch (Renderer::getRenderAPI()) {

		case RendererAPI::API::None:
			RE_CORE_ASSERT(false, "RendererAPI::None is not supported yet"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(filePath);
		}

		RE_CORE_ASSERT(false, "Unknown/Unsupported Renderer API");
		return nullptr;
	}

	Ref<Shader> Shader::create(const std::string& name,
		const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (Renderer::getRenderAPI()) {

		case RendererAPI::API::None:
			RE_CORE_ASSERT(false, "RendererAPI::None is not supported yet"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		RE_CORE_ASSERT(false, "Unknown/Unsupported Renderer API");
		return nullptr;
	}

	void ShaderLibrary::add(const Ref<Shader>& shader) {
		auto& name = shader->getName();
		add(name, shader);
	}

	void ShaderLibrary::add(const std::string& name, const Ref<Shader>& shader) {
		RE_CORE_ASSERT(!isExists(name), "Shader already exists");
		shaders[name] = shader; 
	}

	Ref<Shader> ShaderLibrary::load(const std::string& filePath) {
		auto shader = Shader::create(filePath);
		add(shader);
		return shader;
	}
	Ref<Shader> ShaderLibrary::load(const std::string name, const std::string& filePath) {
		auto shader = Shader::create(filePath);
		add(name, shader);
		return shader;
	}
	Ref<Shader> ShaderLibrary::get(const std::string& name) {
		RE_CORE_ASSERT(shaders.find(name) != shaders.end(), "Shader not found");
		return shaders[name];
	}

	bool ShaderLibrary::isExists(const std::string& name) {
		return shaders.find(name) != shaders.end();
	}
}
