#include "repch.h"

#include "Application.h"
#include "RealEngine/Log.h"

#include "glad/glad.h"

#include "Input.h"

namespace RealEngine {

	Application* Application::appInstance = nullptr;

	static GLenum shaderDataTypeToOpenGLBaseType(SDT type) {
		switch (type) {
			case SDT::None: break;
			
			case SDT::Float:	//fallthrough
			case SDT::Float2:	//fallthrough
			case SDT::Float3:	//fallthrough
			case SDT::Float4:	//fallthrough
			case SDT::Mat3:		//fallthrough
			case SDT::Mat4:		return GL_FLOAT;
			
			case SDT::Int:		//fallthrough
			case SDT::Int2:		//fallthrough
			case SDT::Int3:		//fallthrough
			case SDT::Int4:		return GL_INT;
			
			case SDT::Bool:		return GL_BOOL;
		}
		RE_CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

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


		float vertices[3*3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		vertexBuffer.reset(VertexBuffer::create(vertices, sizeof(vertices)));


		BufferLayout layout = {
			{SDT::Float3, "position"}
		};

		vertexBuffer->setLayout(layout);
		
		uint32_t i = 0;
		for(const auto& l : layout) {
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, 
				l.getComponentCount(), 
				shaderDataTypeToOpenGLBaseType(l.type), 
				l.normalized ? GL_TRUE : GL_FALSE, 
				layout.getStride(), 
				reinterpret_cast<const void *>(l.offset));
			i++;
		}
	

		uint32_t indices[] = {0, 1, 2};

		indexBuffer.reset(IndexBuffer::create(indices, 3));

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
			glDrawElements(GL_TRIANGLES, indexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);

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
