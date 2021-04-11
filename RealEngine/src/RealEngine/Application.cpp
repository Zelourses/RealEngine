#include "repch.h"

#include "Application.h"
#include "RealEngine/Log.h"

#include "GLFW/glfw3.h"

namespace RealEngine {

#define BIND_EVENT_FN(x) std::bind(&x,this, std::placeholders::_1)
	Application::Application()
	{
		window = std::unique_ptr<Window>(Window::create());
		window->setEventCallback(BIND_EVENT_FN(Application::onEvent));
	}

	void Application::onEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		
		RE_CORE_TRACE("{0}",e);
	}
	
	

	void Application::run() {
		while (windowRunning){
			glClearColor(.3f, .2f, .8f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT);
			window->onUpdate();
		}
	}

	Application::~Application()
	{
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		windowRunning = false;
		return true;
	}

	
}
