#pragma once
#include <string>

namespace RealEngine {
	class Shader{
	public:
		
		Shader(const std::string& vertexSrc, const std::string& pixelSrc);
		~Shader();


		void bind() const;
		void unbind() const;

	private:
		//unique identifier of this object
		uint32_t rendererId;
	};
}
