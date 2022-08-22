#pragma once
#include "Real/Renderer/Shader.h"

#include <glm/glm.hpp>

typedef unsigned int GLenum; // just not to make potential abstraction leaks
							 // TODO: remove it from here

namespace Real {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader() override;


		void bind() const override;
		void unbind() const override;


		const std::string& getName() const override { return name; }


		void setInt(const std::string& name, int value) override;
		void setIntArray(const std::string& name, int* values, unsigned count) override;
		void setFloat3(const std::string& name, const glm::vec3& value) override;
		void setFloat4(const std::string& name, const glm::vec4& value) override;
		void setMat4(const std::string& name, const glm::mat4& value) override;


		void uploadUniformInt(const std::string& name, int value) const;
		void uploadUniformIntArray(const std::string& name, int* values, unsigned count) const;
		void uploadUniformFloat(const std::string& name, float value) const;
		void uploadUniformFloat2(const std::string& name, const glm::vec2& vector) const;
		void uploadUniformFloat3(const std::string& name, const glm::vec3& vector) const;
		void uploadUniformFloat4(const std::string& name, const glm::vec4& vector) const;

		void uploadUniformMat3(const std::string& name, const glm::mat3& matrix) const;
		void uploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;

	private:

		std::unordered_map<GLenum, std::string> preProcess(const std::string& shaderSrc);

		void compileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& srcs);
		void compileOrGetOpenGLBinaries();
		void createProgram();
		void reflect(GLenum stage, const std::vector<unsigned>& shaderData);

		//unique identifier of this object (in OpenGL)
		unsigned int rendererId;

		//unique identifier (for humans)
		std::string name;
		std::filesystem::path filepath;
		std::unordered_map<GLenum, std::vector<unsigned>> vulkanSPIRV;
		std::unordered_map<GLenum, std::vector<unsigned>> openGLSPIRV;

		std::unordered_map<GLenum, std::string> openGLSourceCode;
	};
}
