#include <RealEngine.h>


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
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			 -0.75f,  0.75f, 0.0f
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

			out vec3 outPosition; //varying variables, so.. v_... but no

			uniform mat4 viewProjection; // uniform maybe need to mark with u_...
		
			void main(){
				gl_Position = viewProjection * vec4(position,1.0);
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

		shader.reset(new re::Shader(vertexSrc, pixelShader));


		std::string squareVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 position;
		
			uniform mat4 viewProjection;
		
			void main(){
				gl_Position = viewProjection * vec4(position,1.0);
			}
		)";

		std::string squarePixelShader = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
		
			void main(){
				color = vec4(0.3,0.4,0.7,1.0);
			}
		)";
		blueShader.reset(new re::Shader(squareVertexSrc, squarePixelShader));
	}

	void onUpdate() override {

		if (re::Input::isKeyPressed(RE_KEY_LEFT)) {
			cameraPosition.x -= cameraMoveSpeed;
		}
		else if (re::Input::isKeyPressed(RE_KEY_RIGHT)){
			cameraPosition.x += cameraMoveSpeed;
		}
		else if (re::Input::isKeyPressed(RE_KEY_UP)) {
			cameraPosition.y += cameraMoveSpeed;
		}
		else if (re::Input::isKeyPressed(RE_KEY_DOWN)) {
			cameraPosition.y -= cameraMoveSpeed;
		}

		if (re::Input::isKeyPressed(RE_KEY_Q)) {
			cameraRotation += cameraRotationSpeed;
		}

		if (re::Input::isKeyPressed(RE_KEY_E)) {
			cameraRotation -= cameraRotationSpeed;
		}
		
		
		re::RenderCommand::setClearColor({ .2f, .2f, .2f, 1.f });
		re::RenderCommand::clear();

		camera.setPosition(cameraPosition);
		camera.setRotation(cameraRotation);

		re::Renderer::beginScene(camera);

		re::Renderer::submit(blueShader, squareVA);

		re::Renderer::submit(shader, vertexArray);

		re::Renderer::endScene();
	}

	void onEvent(RealEngine::Event& event) override {
		re::EventDispatcher dispatcher(event);
		//dispatcher.dispatch<re::KeyPressedEvent>(RE_BIND_EVENT_FN(ExampleLayer::onKeyPressedEvent));
	}

private:
	std::shared_ptr<re::Shader> shader;
	std::shared_ptr<re::Shader> blueShader;

	std::shared_ptr<re::VertexArray> vertexArray;

	std::shared_ptr<re::VertexArray> squareVA;

	re::OrthographicCamera camera;
	glm::vec3 cameraPosition;
	float cameraMoveSpeed = 0.1f;
	float cameraRotationSpeed = 0.01f;
	float cameraRotation = 0.0f;
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
