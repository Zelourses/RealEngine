#include "repch.h"
#include "WindowsWindow.h"
#include "RealEngine/Events/ApplicationEvent.h"
#include "RealEngine/Events/MouseEvent.h"
#include "RealEngine/Events/KeyEvent.h"

namespace RealEngine{

	static bool isGLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description) {
		RE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}
	
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
			RE_CORE_ASSERT(result, "GLFW initialization failed");
			glfwSetErrorCallback(GLFWErrorCallback);
			isGLFWInitialized = true;
		}

		window = glfwCreateWindow(static_cast<int>(prop.width), 
				static_cast<int>(prop.height), prop.title.c_str(), nullptr,nullptr);
		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, &windowData);
		setVsync(true);
		
		//
		//Set GLFW callbacks
		//
		
		glfwSetWindowSizeCallback(window, [](GLFWwindow* _window, int width, int height) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(_window));

			data.width = width;
			data.height = height;
			
			WindowResizeEvent event(width, height);
			data.eventCallback(event);
		});

		glfwSetWindowCloseCallback(window, [](GLFWwindow* _window) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(_window));
			WindowCloseEvent event;
			data.eventCallback(event);
		});

		glfwSetKeyCallback(window, [](GLFWwindow* _window, int key, int scanCode, int action, int mods) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(_window));
			switch (action) {
				case GLFW_PRESS: {
					KeyPressedEvent event(key, 0);
					data.eventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					KeyReleasedEvent event(key);
					data.eventCallback(event);
					break;
				}

				case GLFW_REPEAT: {
					KeyPressedEvent event(key, 1);
					data.eventCallback(event);
					break;
				}
			}
		});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* _window, int button, int action, int mods) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(_window));

			switch (action) {
				case GLFW_PRESS: {
					MouseButtonPressedEvent event(button);
					data.eventCallback(event);
					break;
				}

				case GLFW_RELEASE: {
					MouseButtonReleasedEvent event(button);
					data.eventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(window, [](GLFWwindow* _window, double xOffset, double yOffset) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(_window));

			MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
			data.eventCallback(event);
		});

		glfwSetCursorPosCallback(window, [](GLFWwindow* _window, double xPos, double yPos) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(_window));

			MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
			data.eventCallback(event);
		});
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
