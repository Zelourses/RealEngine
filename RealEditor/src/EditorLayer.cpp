#include "EditorLayer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <platform/OpenGL/OpenGLShader.h>
#include <entt/entt.hpp>

namespace Real {

	EditorLayer::EditorLayer()
		: Layer("Sandbox2D"), controller(1280.0f / 720.0f) {}

	void EditorLayer::onAttach() {
		checkboardTexture = Texture2D::create("assets/textures/Checkerboard.png");
		spriteSheet		  = Texture2D::create("assets/textures/RPGpack_sheet_2X.png");
		barrelSprite	  = SubTexture2D::createFromCoods(spriteSheet, {8, 2}, {128, 128});

		FramebufferSpecification spec = {
			.width	= 1280,
			.height = 720};
		framebuffer = Framebuffer::create(spec);
		activeScene = createRef<Scene>();
		panel.setContext(activeScene);

		squareEntity = activeScene->createEntity("square");
		squareEntity.add<SpriteRendererComponent>(glm::vec4{0.5f, 0.5f, 0.1f, 1.0f});

		cameraEntity = activeScene->createEntity("Camera");
		cameraEntity.add<CameraComponent>();

		struct CameraController final : ScriptableEntity {
			void onCreate() {
			}


			void onUpdate(Timestep ts) {
				auto&& translation = get<TransformComponent>().translation;
				float  speed	   = 5.0f;

				if (Input::isKeyPressed(KeyCodes::A))
					translation.x -= speed * ts;
				if (Input::isKeyPressed(KeyCodes::D))
					translation.x += speed * ts;
				if (Input::isKeyPressed(KeyCodes::S))
					translation.y -= speed * ts;
				if (Input::isKeyPressed(KeyCodes::W))
					translation.y += speed * ts;
			}

			void onDestroy() {
			}
		};

		cameraEntity.add<NativeScriptComponent>().bind<CameraController>();
	}

	void EditorLayer::onDetach() {}

	void EditorLayer::onUpdate(Real::Timestep ts) {
		RE_PROFILE_FUNCTION();

		if (auto spec = framebuffer->getSpec();
			spec.width != viewportSize.x || spec.height != viewportSize.y) {
			framebuffer->resize(viewportSize);
			controller.resize(viewportSize);

			activeScene->onViewportResize(viewportSize);
		}

		Renderer2D::resetStats();

		controller.onUpdate(ts);

		framebuffer->bind();

		RenderCommand::setClearColor({0.1, 0.1, 0.1, 1.0});
		RenderCommand::clear();

		// Renderer2D::drawQuad({0.0, 0.0, 0.0}, {0.5, 0.5}, barrelSprite);
		activeScene->onUpdate(ts);

		Renderer2D::beginScene(controller.getCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f) {
			for (float x = -5.0f; x < 5.0f; x += 0.5f) {
				glm::vec4 color = {(x + 5.0f) / 10.0f, 0.4, (y + 5.0f) / 10.0f, 1.0};
				Renderer2D::drawQuad({x, y, -0.1f}, {0.45f, 0.45f}, color);
			}
		}
		Renderer2D::endScene();

		framebuffer->unbind();
	}

	void EditorLayer::onImGUIRender() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		auto&& style		  = ImGui::GetStyle();
		auto   minStyle		  = style.WindowMinSize.x;
		style.WindowMinSize.x = 350.0f;
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		ImGui::PopStyleVar();
		style.WindowMinSize.x = minStyle;


		auto stats = Real::Renderer2D::getStats();
		ImGui::Begin("Render statistics");
		ImGui::Text("Renderer2D stats:");
		ImGui::Text("Draw calls: %d", stats.drawCalls);
		ImGui::Text("Quads: %d", stats.quadCount);
		ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
		ImGui::Text("Indices: %d", stats.getTotalIndexCount());
		ImGui::End();

		panel.onImGUIRender();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("Viewport");
		auto res	 = ImGui::GetContentRegionAvail();
		viewportSize = {res.x, res.y};

		auto  textureId = framebuffer->getColorAttachmentID();
		void* texId		= static_cast<uint8_t*>(0) + textureId;
		ImGui::Image(texId, {viewportSize.x, viewportSize.y}, {0, 1}, {1, 0});
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorLayer::onEvent(Real::Event& e) {
		controller.onEvent(e);
	}
}