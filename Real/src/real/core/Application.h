#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "real/Events/Event.h"
#include "real/Events/ApplicationEvent.h"

#include "real/Core/Timestep.h"

#include "real/ImGUI/ImGUILayer.h"

namespace Real {

	class Application {
	public:
		void run();

		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);

		inline static Application& getApplication() { return *appInstance; }

		inline Window& getWindow() const { return *window; }

		Application(const WindowProps& props = WindowProps());
		virtual ~Application();

	private:
		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);

		bool windowRunning     = true;
		bool windowIsMinimized = false;

		Real::Scope<Window> window;
		ImGUILayer*         imGUILayer;
		LayerStack          layerStack;
		static Application* appInstance;

		float lastFrameTime = 0.0f;
	};

	// Client definition
	Application* createApplication();
}