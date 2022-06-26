#include "EditorLayer.h"

#include <imgui/imgui.h>

#include <ImGuizmo.h>
#include <entt/entt.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "real/scene/SceneSerializer.h"
#include "platform/OpenGL/OpenGLShader.h"
#include "real/utils/PlatformUtils.h"
#include "real/math/Math.h"

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
		sceneHierarchyPanel.setContext(activeScene);

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

				if (Input::isKeyPressed(KeyCode::A))
					translation.x -= speed * ts;
				if (Input::isKeyPressed(KeyCode::D))
					translation.x += speed * ts;
				if (Input::isKeyPressed(KeyCode::S))
					translation.y -= speed * ts;
				if (Input::isKeyPressed(KeyCode::W))
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
		RE_PROFILE_FUNCTION();

		auto   windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		auto&& viewport	   = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		windowFlags |= ImGuiWindowFlags_NoTitleBar
					 | ImGuiWindowFlags_NoCollapse
					 | ImGuiWindowFlags_NoResize
					 | ImGuiWindowFlags_NoMove
					 | ImGuiWindowFlags_NoBringToFrontOnFocus
					 | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Dockspace", static_cast<bool*>(0), windowFlags);
		ImGui::PopStyleVar(); // ImGuiStyleVar_WindowPadding

		ImGui::PopStyleVar(2); //ImGuiStyleVar_WindowRounding and ImGuiStyleVar_WindowBorderSize

		auto&& style		  = ImGui::GetStyle();
		auto   minStyle		  = style.WindowMinSize.x;
		style.WindowMinSize.x = 300.0f;

		auto dockspaceID = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspaceID);

		style.WindowMinSize.x = minStyle;

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New")) {
					newScene();
				}
				if (ImGui::MenuItem("Open...", "Ctrl+O")) {
					openScene();
				}
				if (ImGui::MenuItem("Save As...", "Ctrl+S")) {
					saveSceneAs();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}


		auto stats = Real::Renderer2D::getStats();
		ImGui::Begin("Render statistics");
		{
			ImGui::Text("Renderer2D stats:");
			ImGui::Text("Draw calls: %d", stats.drawCalls);
			ImGui::Text("Quads: %d", stats.quadCount);
			ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
			ImGui::Text("Indices: %d", stats.getTotalIndexCount());
		}
		ImGui::End(); //Render statistics

		sceneHierarchyPanel.onImGUIRender();


		//Main window renders here
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");
		{
			auto res			 = ImGui::GetContentRegionAvail();
			auto viewPortFocused = ImGui::IsWindowFocused();
			auto viewPortHovered = ImGui::IsWindowHovered();
			Application::getApplication().getImGuiLayer().blockEvents(!viewPortFocused && !viewPortHovered);
			viewportSize = {res.x, res.y};

			auto  textureId = framebuffer->getColorAttachmentID();
			void* texId		= static_cast<uint8_t*>(0) + textureId;
			ImGui::Image(texId, {viewportSize.x, viewportSize.y}, {0, 1}, {1, 0});


			//Gizmo stuff
			auto selected = sceneHierarchyPanel.selectedEntity();
			if (selected && gizmoType != -1) {
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();
				auto size		  = ImGui::GetWindowPos();
				auto windowHeight = ImGui::GetWindowHeight();
				auto windowWidth  = ImGui::GetWindowWidth();
				ImGuizmo::SetRect(size.x, size.y, windowWidth, windowHeight);

				//Camera
				auto   cameraEntity	 = activeScene->getPrimaryCamera();
				auto&& cam			 = cameraEntity.get<CameraComponent>().camera;
				auto&& camProjection = cam.projection();
				auto&& camView		 = glm::inverse(cameraEntity.get<TransformComponent>().transform());

				// Entity transform
				auto&& tc		 = selected.get<TransformComponent>();
				auto   transform = tc.transform();

				bool  isSnap  = Input::isKeyPressed(KeyCode::LEFT_CONTROL);
				float snapVal = 0.5f;
				if (gizmoType == ImGuizmo::OPERATION::ROTATE) {
					snapVal = 45.0f;
				}

				float snapVals[3]{snapVal, snapVal, snapVal};

				ImGuizmo::Manipulate(glm::value_ptr(camView),
									 glm::value_ptr(camProjection),
									 static_cast<ImGuizmo::OPERATION>(gizmoType),
									 ImGuizmo::LOCAL,
									 glm::value_ptr(transform),
									 nullptr,
									 isSnap ? snapVals : nullptr);

				if (ImGuizmo::IsUsing()) {
					glm::vec3 translation, rotation, scale;
					if (Math::decomposeTransfrom(transform, translation, rotation, scale)) {
						//To get rid of gimbal lock
						auto deltaRotaion = rotation - tc.rotation;
						tc.translation	  = translation;
						tc.rotation += deltaRotaion;
						tc.scale = scale;
					}
				}
			}
		}
		ImGui::End();		  // Viewport
		ImGui::PopStyleVar(); //ImGuiStyleVar_WindowPadding


		ImGui::End(); //Dockspace
	}

	void EditorLayer::onEvent(Real::Event& e) {
		controller.onEvent(e);
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<KeyPressedEvent>(RE_BIND_EVENT_FN(EditorLayer::onKeyPressed));
	}
	bool EditorLayer::onKeyPressed(KeyPressedEvent e) {
		if (e.getRepeatCount() > 0) {
			return false;
		}
		bool controlPressed = Input::isKeyPressed(KeyCode::LEFT_CONTROL)
						   || Input::isKeyPressed(KeyCode::RIGHT_CONTROL);
		bool shiftPressed = Input::isKeyPressed(KeyCode::LEFT_SHIFT)
						 || Input::isKeyPressed(KeyCode::RIGHT_SHIFT);
		switch (e.getKeyCode()) {
			case KeyCode::S: {
				if (controlPressed) {
					saveSceneAs();
				}
				break;
			}
			case KeyCode::O: {
				if (controlPressed) {
					openScene();
				}
				break;
			}

			//Gizmos things
			case KeyCode::Q: {
				gizmoType = -1;
				break;
			}
			case KeyCode::W: {
				gizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case KeyCode::E: {
				gizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case KeyCode::R: {
				gizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}
		}
		return true;
	}
	void EditorLayer::newScene() {
		activeScene = createRef<Scene>();
		activeScene->onViewportResize(viewportSize);
		sceneHierarchyPanel.setContext(activeScene);
	}
	void EditorLayer::openScene() {
		auto filepath = FileDialogs::openFile("Real Engine serialized scene (*.ress)\0*.ress\0");
		if (filepath) {
			activeScene = createRef<Scene>();
			activeScene->onViewportResize(viewportSize);
			sceneHierarchyPanel.setContext(activeScene);

			SceneSerializer serializer(activeScene);
			serializer.deserialize(*filepath);
		}
	}
	void EditorLayer::saveSceneAs() {
		auto filepath = FileDialogs::saveFile("Real Engine Serialized Scene (*.ress)\0*.ress\0");

		if (filepath) {
			SceneSerializer serializer(activeScene);
			serializer.serialize(*filepath);
		}
	}
}