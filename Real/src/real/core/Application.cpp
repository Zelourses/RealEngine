#include "repch.h"

#include "Application.h"
#include "real/core/debug/Log.h"

#include "real/renderer/Renderer.h"

#include "Input.h"
#include "GLFW/glfw3.h"

namespace Real {

	Application* Application::appInstance = nullptr;


	Application::Application(const WindowProps& props) {
		RE_CORE_ASSERT(!appInstance, "Creating already existing application!");
		appInstance = this;

		window = std::unique_ptr<Window>(Window::create(props));
		window->setEventCallback(RE_BIND_EVENT_FN(Application::onEvent));

		Renderer::init();

		imGUILayer = new ImGUILayer;
		pushOverlay(imGUILayer);
	}

	void Application::onEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(RE_BIND_EVENT_FN(Application::onWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(RE_BIND_EVENT_FN(Application::onWindowResize));

		for (auto it = layerStack.end(); it != layerStack.begin();) {
			(*--it)->onEvent(e);
			if (e.handled)
				break;
		}
	}

	void Application::run() {
		while (windowRunning) {
			const float time = static_cast<float>(glfwGetTime()); // FIXME:Temp things.
																  // Need to change to something like: Platform::getTime();

			Timestep timestep = time - lastFrameTime;
			lastFrameTime	  = time;

			if (!windowIsMinimized) {
				for (Layer* layer: layerStack) {
					layer->onUpdate(timestep);
				}
			}

			// ImGUI layers is here because they can use docking space and while main window is minimized, they will still work
			// If this will be inside upper `if` statement all windows will stop responding
			imGUILayer->begin();
			for (Layer* layer: layerStack) {
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

	Application::~Application() {
	}

	bool Application::onWindowClose(WindowCloseEvent& e) {
		windowRunning = false;
		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent& e) {
		if (e.getWidth() == 0 || e.getHeight() == 0) {
			windowIsMinimized = true;
		} else {
			windowIsMinimized = false;

			Renderer::onWindowResize(e.getWidth(), e.getHeight());
		}


		return false;
	}


}
