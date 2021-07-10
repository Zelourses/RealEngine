#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "RealEngine/Events/ApplicationEvent.h"

#include "RealEngine/ImGUI/ImGUILayer.h"

//FIXME: leaking abstraction
#include "RealEngine/Renderer/Shader.h"

namespace RealEngine {
	class RE_API Application {
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
		
		std::unique_ptr<Window> window;
		ImGUILayer* imGUILayer;
		bool windowRunning = true;

		LayerStack layerStack;

		static Application* appInstance;

		unsigned int vertexArray, vertexBuffer, indexBuffer; //FIXME: temp things
		std::unique_ptr<Shader> shader;
	};

	//Client definition
	Application* createApplication();
}
