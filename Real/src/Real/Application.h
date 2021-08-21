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
		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);
		
		bool windowRunning = true;
		bool windowIsResized = false;
		
		Real::Scope<Window> window;
		ImGUILayer* imGUILayer;
		LayerStack layerStack;
		static Application* appInstance;

		float lastFrameTime = 0.0f;
	};

	//Client definition
	Application* createApplication();
}
