#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "RealEngine/Events/ApplicationEvent.h"

#include "RealEngine/ImGUI/ImGUILayer.h"

//FIXME: leaking abstraction. We don't really need to work with shader in main app, right?
#include "RealEngine/Renderer/Shader.h"

#include "RealEngine/Renderer/Buffer.h"
#include "RealEngine/Renderer/VertexArray.h"

#include "RealEngine/Renderer/OrthographicCamera.h"

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
		
		std::shared_ptr<Shader> shader;
		std::shared_ptr<Shader> blueShader;
		
		std::shared_ptr<VertexArray> vertexArray;

		std::shared_ptr<VertexArray> squareVA;

		OrthographicCamera camera;
	};

	//Client definition
	Application* createApplication();
}
