#include "repch.h"

#include "Application.h"
#include "RealEngine/Log.h"

#include "glad/glad.h"

#include "Input.h"

namespace RealEngine {

	Application* Application::appInstance = nullptr;

	Application::Application() {
		RE_CORE_ASSERT(!appInstance, "Creating already existing application!");
		appInstance = this;
		
		window = std::unique_ptr<Window>(Window::create());
		window->setEventCallback(RE_BIND_EVENT_FN(Application::onEvent));
		imGUILayer = new ImGUILayer;
		pushOverlay(imGUILayer);


		// temp things
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		float vertices[3*3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 3*sizeof(float), nullptr);

		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		unsigned int indices[] = {0, 1, 2};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 position;

			out vec3 outPosition; //varying variables, so.. v_... but no
		
			void main(){
				gl_Position = vec4(position,1.0);
				outPosition = position;
				
			}
		)";

		std::string pixelShader = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 outPosition;
		
			void main(){
				color = vec4(outPosition* 0.5 + 0.5, 1.0);
			}
		)";
		
		shader.reset(new Shader(vertexSrc, pixelShader));
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
			glClearColor(.2f, .2f, .2f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT);

			shader->bind();
			glBindVertexArray(vertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

			for(Layer* layer: layerStack) {
				layer->onUpdate();
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
