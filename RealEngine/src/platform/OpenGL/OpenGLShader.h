#pragma once
#include "RealEngine/Renderer/Shader.h"

#include <glm/glm.hpp>
namespace RealEngine{
	class OpenGLShader : public Shader
	{
	public:
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
		//unique identifier of this object
		uint32_t rendererId;
	};
}

