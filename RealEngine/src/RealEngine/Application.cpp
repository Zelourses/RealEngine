#include "Application.h"
#include "RealEngine/Events/ApplicationEvent.h"
#include "RealEngine/Log.h"

namespace RealEngine {
	Application::Application()
	{
	}
	Application::~Application()
	{
	}
	void Application::run() {
		WindowResizeEvent event(1920, 1080); //it works!
		RE_TRACE(event);
		while (true){
			
		}
	}
}
