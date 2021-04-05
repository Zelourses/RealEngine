#include "repch.h"
#include "WindowsWindow.h"

namespace RealEngine{

	static bool isGLFWInitialized = false;
	
	Window* Window::create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {
		WindowsWindow::init(props);
	}

	WindowsWindow::~WindowsWindow() {
		WindowsWindow::shutdown();
	}

	void WindowsWindow::init(const WindowProps& prop) {
		windowData.title = prop.title;
		windowData.width = prop.width;
		windowData.height = prop.height;

		RE_CORE_INFO("Initializing window: {0} {1} {2}", prop.title, prop.width, prop.height);

		if (!isGLFWInitialized) {

			//TODO: glfwTerminate();
			const int result = glfwInit();
			RE_CORE_ASSERT(result, "GLFW initialization failed")
			
			isGLFWInitialized = true;
		}

		window = glfwCreateWindow(static_cast<int>(prop.width), 
				static_cast<int>(prop.height), prop.title.c_str(), nullptr,nullptr);
		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, &windowData);
		setVsync(true);
		
	}

	void WindowsWindow::shutdown() {
		glfwDestroyWindow(window);
	}

	void WindowsWindow::onUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	void WindowsWindow::setVsync(const bool enabled) {
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		
		windowData.Vsync = enabled;
	}

	bool WindowsWindow::isVsync() const {
		return windowData.Vsync;
	}

	unsigned WindowsWindow::getWidth() const {
		return windowData.width;
	}

	unsigned WindowsWindow::getHeight() const {
		return windowData.height;
	}





}
