#include <Real.h>


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

#include "platform/OpenGL/OpenGLShader.h"

//FIXME: still a little bit of leaking abstraction. Need to move shaders somewhere
class ExampleLayer : public Real::Layer {
public:
	ExampleLayer()
		: Layer("Example"), camera(-1.6f, 1.6f, -0.9f, 0.9f),
		cameraPosition(0.0f, 0.0f, 0.0f) {

		vertexArray.reset(Real::VertexArray::create());


		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};
		Real::Ref<Real::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Real::VertexBuffer::create(vertices, sizeof(vertices)));


		Real::BufferLayout layout = {
			{Real::SDT::Float3, "position"}
		};

		vertexBuffer->setLayout(layout);

		vertexArray->addVertexBuffer(vertexBuffer);

		unsigned int indices[] = { 0, 1, 2 };
		Real::Ref<Real::IndexBuffer> indexBuffer;
		indexBuffer.reset(Real::IndexBuffer::create(indices, 3));

		vertexArray->setIndexBuffer(indexBuffer);

		float squareVertices[] = {
			-0.5f, -0.5f, 0.0f, /*texture coordinates*/	0.0f, 0.0f, //first point
			 0.5f, -0.5f, 0.0f,							1.0f, 0.0f, //second point
			 0.5f,  0.5f, 0.0f,							1.0f, 1.0f, //third point
			-0.5f,  0.5f, 0.0f,							0.0f, 1.0f	//fourth point
		};



		squareVA.reset(Real::VertexArray::create());

		Real::Ref<Real::VertexBuffer> squareVB;
		squareVB.reset(Real::VertexBuffer::create(squareVertices, sizeof(squareVertices)));

		squareVB->setLayout({
			{Real::SDT::Float3, "position"},
			{Real::SDT::Float2, "textCoordinates"}
			});
		squareVA->addVertexBuffer(squareVB);

		unsigned int squareIndices[] = { 0, 1, 2, 2, 3, 0 };

		Real::Ref<Real::IndexBuffer> squareIB;
		squareIB.reset(Real::IndexBuffer::create(squareIndices, sizeof(squareIndices)));

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

		shader = Real::Shader::create("triangleShader", vertexSrc, pixelShader);


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
		squareShader = Real::Shader::create("squareShader", squareVertexSrc, squarePixelShader);


		auto textureShader = shaderLibrary.load("assets/shaders/Texture.glsl");


		std::dynamic_pointer_cast<Real::OpenGLShader>(textureShader)->bind();
		std::dynamic_pointer_cast<Real::OpenGLShader>(textureShader)->uploadUniformInt("Texture", 0);
		
		texture = Real::Texture2D::create("assets/textures/Checkerboard.png");
		cppLogo = Real::Texture2D::create("assets/textures/cpp_logo.png");
	}

	void onUpdate(Real::Timestep timestep) override {

		if (Real::Input::isKeyPressed(RE_KEY_LEFT)) {
			cameraPosition.x -= cameraMoveSpeed * timestep;
		}
		else if (Real::Input::isKeyPressed(RE_KEY_RIGHT)){
			cameraPosition.x += cameraMoveSpeed * timestep;
		}
		if (Real::Input::isKeyPressed(RE_KEY_UP)) {
			cameraPosition.y += cameraMoveSpeed * timestep;
		}
		else if (Real::Input::isKeyPressed(RE_KEY_DOWN)) {
			cameraPosition.y -= cameraMoveSpeed * timestep;
		}

		if (Real::Input::isKeyPressed(RE_KEY_Q)) {
			cameraRotation += cameraRotationSpeed * timestep;
		} else if (Real::Input::isKeyPressed(RE_KEY_E)) {
			cameraRotation -= cameraRotationSpeed * timestep;
		}

		Real::RenderCommand::setClearColor({ .2f, .2f, .2f, 1.f });
		Real::RenderCommand::clear();

		camera.setPosition(cameraPosition);
		camera.setRotation(cameraRotation);

		Real::Renderer::beginScene(camera);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		glm::vec4 redColor(0.7f, 0.3f, 0.4f, 1.0f);
		glm::vec4 blueColor(0.3f, 0.4f, 0.7f, 1.0f);

		/*
		 * Example of Material system API
		 * ... squareMesh ...;
		 * Real::MaterialRef material = new Real::Material(squareShader);
		 * Real::MaterialInstanceRef materialInstance = new Real::MaterialInstance(material);
		 * materialInstance.set("Color", redColor);
		 *
		 *
		 * squareMesh->setMaterial(material);
		*/
		std::dynamic_pointer_cast<Real::OpenGLShader>(squareShader)->bind();
		std::dynamic_pointer_cast<Real::OpenGLShader>(squareShader)->uploadUniformFloat3("Color",squareColor);
		
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 position(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 tranform = glm::translate(glm::mat4(1.0f), position) * scale;
				Real::Renderer::submit(squareShader, squareVA, tranform);
			}
		}
		auto textureShader = shaderLibrary.get("Texture");
		texture->bind();
		Real::Renderer::submit(textureShader, squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		cppLogo->bind();
		Real::Renderer::submit(textureShader, squareVA, 
			glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Real::Renderer::endScene();
	}

	void onImGUIRender() override {
		ImGui::Begin("square settings");
		ImGui::ColorEdit3("Square color", glm::value_ptr(squareColor));
		ImGui::End();
	}

private:
	Real::Ref<Real::Shader> shader;
	Real::ShaderLibrary shaderLibrary;
	Real::Ref<Real::Shader> squareShader;
	Real::Ref<Real::VertexArray> vertexArray;
	Real::Ref<Real::VertexArray> squareVA;

	Real::Ref<Real::Texture2D> texture, cppLogo;

	Real::OrthographicCamera camera;
	glm::vec3 cameraPosition;
	float cameraMoveSpeed = 1.0f;
	float cameraRotationSpeed = 1.0f;
	float cameraRotation = 0.0f;

	glm::vec3 squareColor = { 0.3f, 0.4f, 0.7f};
};

class Sandbox : public Real::Application {
public:
	Sandbox() {
		pushLayer(new ExampleLayer());
	}
	~Sandbox() {

	}

};

Real::Application* Real::createApplication() {
	return new Sandbox();
}
