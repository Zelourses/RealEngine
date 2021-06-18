#pragma once
#include "RealEngine/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace RealEngine {
	class OpenGLContext : public GraphicsContext {
	public:

		OpenGLContext(GLFWwindow* windowHandle);
		
		virtual void init() override;
		virtual void swapBuffers() override;

	private:
		GLFWwindow* windowHandle;
	};
}
