#include "repch.h"

#include "Application.h"
#include "Real/Log.h"

#include "Real/Renderer/Renderer.h"

#include "Input.h"
#include "GLFW/glfw3.h"

namespace Real {

	Application* Application::appInstance = nullptr;

	

	Application::Application() {
		RE_CORE_ASSERT(!appInstance, "Creating already existing application!");
		appInstance = this;
		
		window = std::unique_ptr<Window>(Window::create());
		window->setEventCallback(RE_BIND_EVENT_FN(Application::onEvent));
		imGUILayer = new ImGUILayer;
		pushOverlay(imGUILayer);

		
	}

	void Application::onEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(RE_BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = layerStack.end(); it != layerStack.begin();) {
			(*--it)->onEvent(e);
			if (e.handled)
				break;
		}
	}

	void Application::run() {
		while (windowRunning){

			const float time = static_cast<float>(glfwGetTime()); //FIXME:Temp things.
								//Need to change to something like: Platform::getTime();
			
			Timestep timestep = time - lastFrameTime;
			lastFrameTime = time;
			
			for(Layer* layer: layerStack) {
				layer->onUpdate(timestep);
			}
			imGUILayer->begin();
			for (Layer* layer : layerStack) {
				layer->onImGUIRender();
			}
			imGUILayer->end();
			window->onUpdate();
		}
	}

	void Application::pushLayer(Layer* layer) {
		layerStack.pushLayer(layer);
		layer->onAttach();
	}
	void Application::pushOverlay(Layer* overlay) {

		layerStack.pushOverlay(overlay);
		overlay->onAttach();
	}

	Application::~Application()
	{
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		windowRunning = false;
		return true;
	}

	
}
