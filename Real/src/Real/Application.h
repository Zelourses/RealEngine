#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "Real/Events/ApplicationEvent.h"

#include "Real/Core/Timestep.h"

#include "Real/ImGUI/ImGUILayer.h"

namespace Real {
	class Application {
	public:
		void run();

		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);

		inline static Application& getApplication() { return *appInstance; }

		inline Window& getWindow() const { return *window; }
		
		Application();
		virtual ~Application();
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		
		bool windowRunning = true;
		
		std::unique_ptr<Window> window;
		ImGUILayer* imGUILayer;
		LayerStack layerStack;
		static Application* appInstance;

		float lastFrameTime = 0.0f;
	};

	//Client definition
	Application* createApplication();
}
