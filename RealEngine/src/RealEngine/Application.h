#pragma once

#include "Core.h"

namespace RealEngine {
	class RE_API Application {
	public:
		void run();
		Application();
		virtual ~Application();
	};

	//Client definition
	Application* createApplication();
}
