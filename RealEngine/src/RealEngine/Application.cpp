#include "repch.h"

#include "Application.h"
#include "RealEngine/Events/ApplicationEvent.h"
#include "RealEngine/Log.h"

#include "GLFW/glfw3.h"

namespace RealEngine {
	Application::Application()
	{
		window = std::unique_ptr<Window>(Window::create());
	}
	Application::~Application()
	{
	}
	void Application::run() {
		while (windowRunning){
			glClearColor(.3f, .2f, .8f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT);
			window->onUpdate();
		}
	}
}
