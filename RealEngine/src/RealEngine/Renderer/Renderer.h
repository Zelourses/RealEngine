#pragma once

namespace RealEngine {
	enum class RendererAPI {
		None = 0,
		OpenGL = 1
	};


	class Renderer {
	public:
		inline static RendererAPI getRenderAPI() { return rendererAPI; }
	private:
		static RendererAPI rendererAPI;
	};
}
