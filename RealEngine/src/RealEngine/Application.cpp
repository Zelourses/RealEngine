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

		vertexArray.reset(VertexArray::create());


		float vertices[3*3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::create(vertices, sizeof(vertices)));


		BufferLayout layout = {
			{SDT::Float3, "position"}
		};

		vertexBuffer->setLayout(layout);

		vertexArray->addVertexBuffer(vertexBuffer);
		
		uint32_t indices[] = {0, 1, 2};
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::create(indices, 3));

		vertexArray->setIndexBuffer(indexBuffer);

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			 -0.75f,  0.75f, 0.0f
		};

		

		squareVA.reset(VertexArray::create());

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::create(squareVertices, sizeof(squareVertices)));
		
		squareVB->setLayout({
			{SDT::Float3, "position"}
			});
		squareVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[] = { 0, 1, 2, 2, 3, 0};

		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::create(squareIndices, sizeof(squareIndices)));

		squareVA->setIndexBuffer(squareIB);
		

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


		std::string squareVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 position;
		
			void main(){
				gl_Position = vec4(position,1.0);
			}
		)";

		std::string squarePixelShader = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
		
			void main(){
				color = vec4(0.3,0.4,0.7,1.0);
			}
		)";
		blueShader.reset(new Shader(squareVertexSrc, squarePixelShader));
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

			blueShader->bind();
			squareVA->bind();
			glDrawElements(GL_TRIANGLES, squareVA->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
			
			shader->bind();
			vertexArray->bind();
			glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);

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
