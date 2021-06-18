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
	}
	void OpenGLContext::swapBuffers() {
		glfwSwapBuffers(windowHandle);
	}
}
