#include <RealEngine.h>


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

#include "platform/OpenGL/OpenGLShader.h"

//FIXME: still a little bit of leaking abstraction. Need to move shaders somewhere
class ExampleLayer : public RealEngine::Layer {
public:
	ExampleLayer()
		: Layer("Example"), camera(-1.6f, 1.6f, -0.9f, 0.9f),
		cameraPosition(0.0f, 0.0f, 0.0f) {

		vertexArray.reset(re::VertexArray::create());


		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};
		std::shared_ptr<re::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(re::VertexBuffer::create(vertices, sizeof(vertices)));


		re::BufferLayout layout = {
			{re::SDT::Float3, "position"}
		};

		vertexBuffer->setLayout(layout);

		vertexArray->addVertexBuffer(vertexBuffer);

		uint32_t indices[] = { 0, 1, 2 };
		std::shared_ptr<re::IndexBuffer> indexBuffer;
		indexBuffer.reset(re::IndexBuffer::create(indices, 3));

		vertexArray->setIndexBuffer(indexBuffer);

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};



		squareVA.reset(re::VertexArray::create());

		std::shared_ptr<re::VertexBuffer> squareVB;
		squareVB.reset(re::VertexBuffer::create(squareVertices, sizeof(squareVertices)));

		squareVB->setLayout({
			{re::SDT::Float3, "position"}
			});
		squareVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[] = { 0, 1, 2, 2, 3, 0 };

		std::shared_ptr<re::IndexBuffer> squareIB;
		squareIB.reset(re::IndexBuffer::create(squareIndices, sizeof(squareIndices)));

		squareVA->setIndexBuffer(squareIB);


		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 position;

			uniform mat4 viewProjection; // uniform maybe need to mark with u_...
			uniform mat4 transform;
		
			out vec3 outPosition; //varying variables, so.. v_... but no
		
			void main(){
				gl_Position = viewProjection * transform *vec4(position,1.0);
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

		shader.reset(re::Shader::create(vertexSrc, pixelShader));


		std::string squareVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 position;
		
			uniform mat4 viewProjection;
			uniform mat4 transform;
		
			void main(){
				gl_Position = viewProjection * transform * vec4(position,1.0);
			}
		)";

		std::string squarePixelShader = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			uniform vec3 Color;
		
			void main(){
				color = vec4(Color, 1.0f);
			}
		)";
		squareShader.reset(re::Shader::create(squareVertexSrc, squarePixelShader));
	}

	void onUpdate(re::Timestep timestep) override {

		RE_TRACE("Delta time: {0} ({1})", timestep.getSeconds(), timestep.getMilliseconds());

		if (re::Input::isKeyPressed(RE_KEY_LEFT)) {
			cameraPosition.x -= cameraMoveSpeed * timestep;
		}
		else if (re::Input::isKeyPressed(RE_KEY_RIGHT)){
			cameraPosition.x += cameraMoveSpeed * timestep;
		}
		if (re::Input::isKeyPressed(RE_KEY_UP)) {
			cameraPosition.y += cameraMoveSpeed * timestep;
		}
		else if (re::Input::isKeyPressed(RE_KEY_DOWN)) {
			cameraPosition.y -= cameraMoveSpeed * timestep;
		}

		if (re::Input::isKeyPressed(RE_KEY_Q)) {
			cameraRotation += cameraRotationSpeed * timestep;
		} else if (re::Input::isKeyPressed(RE_KEY_E)) {
			cameraRotation -= cameraRotationSpeed * timestep;
		}

		re::RenderCommand::setClearColor({ .2f, .2f, .2f, 1.f });
		re::RenderCommand::clear();

		camera.setPosition(cameraPosition);
		camera.setRotation(cameraRotation);

		re::Renderer::beginScene(camera);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		glm::vec4 redColor(0.7f, 0.3f, 0.4f, 1.0f);
		glm::vec4 blueColor(0.3f, 0.4f, 0.7f, 1.0f);

		/*
		 * Example of Material system API
		 * ... squareMesh ...;
		 * re::MaterialRef material = new re::Material(squareShader);
		 * re::MaterialInstanceRef materialInstance = new re::MaterialInstance(material);
		 * materialInstance.set("Color", redColor);
		 *
		 *
		 * squareMesh->setMaterial(material);
		*/
		std::dynamic_pointer_cast<re::OpenGLShader>(squareShader)->bind();
		std::dynamic_pointer_cast<re::OpenGLShader>(squareShader)->uploadUniformFloat3("Color",squareColor);
		
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 position(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 tranform = glm::translate(glm::mat4(1.0f), position) * scale;
				re::Renderer::submit(squareShader, squareVA, tranform);
			}
		}

		re::Renderer::submit(shader, vertexArray);

		re::Renderer::endScene();
	}

	void onImGUIRender() override {
		ImGui::Begin("square settings");
		ImGui::ColorEdit3("Square color", glm::value_ptr(squareColor));
		ImGui::End();
	}

private:
	std::shared_ptr<re::Shader> shader;
	std::shared_ptr<re::Shader> squareShader;

	std::shared_ptr<re::VertexArray> vertexArray;

	std::shared_ptr<re::VertexArray> squareVA;

	re::OrthographicCamera camera;
	glm::vec3 cameraPosition;
	float cameraMoveSpeed = 1.0f;
	float cameraRotationSpeed = 1.0f;
	float cameraRotation = 0.0f;

	glm::vec3 squareColor = { 0.3f, 0.4f, 0.7f};
};

class Sandbox : public RealEngine::Application {
public:
	Sandbox() {
		pushLayer(new ExampleLayer());
	}
	~Sandbox() {

	}

};

RealEngine::Application* RealEngine::createApplication() {
	return new Sandbox();
}
