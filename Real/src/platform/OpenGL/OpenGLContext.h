#pragma once
#include "Real/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Real {
	class OpenGLContext : public GraphicsContext {
	public:

		OpenGLContext(GLFWwindow* windowHandle);
		
		virtual void init() override;
		virtual void swapBuffers() override;

	private:
		GLFWwindow* windowHandle;
	};
}
