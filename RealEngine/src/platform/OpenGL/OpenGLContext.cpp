#include "repch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>

#include "glad/glad.h"

namespace RealEngine {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: windowHandle(windowHandle){ RE_CORE_ASSERT(windowHandle, "Handle is NULL"); }
	void OpenGLContext::init() {

		glfwMakeContextCurrent(windowHandle);

		int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		RE_CORE_ASSERT(status, "Failed to initialize Glad");

		RE_CORE_INFO("OpenGL info:");
		RE_CORE_INFO("  Vendor: {0}",glGetString(GL_VENDOR));
		RE_CORE_INFO("  Renderer: {0}",glGetString(GL_RENDERER));
		RE_CORE_INFO("  Version: {0}",glGetString(GL_VERSION));
	}
	void OpenGLContext::swapBuffers() {
		glfwSwapBuffers(windowHandle);
	}
}
