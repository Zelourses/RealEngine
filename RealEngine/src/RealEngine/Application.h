#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"

namespace RealEngine {
	class RE_API Application {
	public:
		void run();

		void onEvent(Event& e);
		
		Application();
		virtual ~Application();
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		
		std::unique_ptr<Window> window;
		bool windowRunning = true;
	};

	//Client definition
	Application* createApplication();
}
