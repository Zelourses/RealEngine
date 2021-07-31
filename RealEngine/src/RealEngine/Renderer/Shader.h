#pragma once
#include <string>
#include <glm/glm.hpp>

namespace RealEngine {
	class Shader{
	public:
		
		Shader(const std::string& vertexSrc, const std::string& pixelSrc);
		~Shader();


		void bind() const;
		void unbind() const;

		void uploadUniformFloat4(const std::string& name,const glm::vec4& vector);
		void uploadUniformMat4(const std::string& name,const glm::mat4& matrix);
	private:
		//unique identifier of this object
		uint32_t rendererId;
	};
}
