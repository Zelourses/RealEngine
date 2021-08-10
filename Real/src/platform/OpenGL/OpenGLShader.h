#pragma once
#include "Real/Renderer/Shader.h"

#include <glm/glm.hpp>

typedef unsigned int GLenum; // just not to make potential abstraction leaks
							// TODO: remove it from here

namespace Real{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& vertexSrc, const std::string& pixelSrc);
		~OpenGLShader() override;


		void bind() const override;
		void unbind() const override;

		void uploadUniformInt(const std::string& name, int value) const;
		
		void uploadUniformFloat(const std::string& name, float value) const;
		void uploadUniformFloat2(const std::string& name, const glm::vec2& vector) const;
		void uploadUniformFloat3(const std::string& name, const glm::vec3& vector) const;
		void uploadUniformFloat4(const std::string& name, const glm::vec4& vector) const;
		
		void uploadUniformMat3(const std::string& name, const glm::mat3& matrix) const;
		void uploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;
	private:
		std::string readFile(const std::string& filePath);

		std::unordered_map<GLenum, std::string> preProcess(const std::string& shaderSrc);
		
		void compile(const std::unordered_map<GLenum, std::string> shaderSrc);
		
		//unique identifier of this object
		unsigned int rendererId;
	};
}

