#pragma once

#include "Core.h"
#include "Window.h"

namespace RealEngine {
	class RE_API Application {
	public:
		void run();
		
		Application();
		virtual ~Application();
	private:
		std::unique_ptr<Window> window;
		bool windowRunning = true;
	};

	//Client definition
	Application* createApplication();
}
