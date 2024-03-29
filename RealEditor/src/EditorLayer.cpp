#include "EditorLayer.h"

#include <imgui/imgui.h>

#include <ImGuizmo.h>
#include <entt/entt.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "real/math/Math.h"
#include "real/utils/PlatformUtils.h"
#include "real/scene/SceneSerializer.h"
#include "platform/OpenGL/OpenGLShader.h"

namespace Real {

	EditorLayer::EditorLayer()
		: Layer("RealEditor main layer"), camera(glm::radians(30.0f), 1.778f, 0.1f, 1000.0f) {}

	void EditorLayer::onAttach() {
		FramebufferSpecification spec = {
			.width		 = 1280,
			.height		 = 720,
			.attachments = {FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::DEPTH}};
		framebuffer = Framebuffer::create(spec);
		activeScene = createRef<Scene>();
		sceneHierarchyPanel.setContext(activeScene);
	}

	void EditorLayer::onDetach() {}

	void EditorLayer::onUpdate(Real::Timestep ts) {
		RE_PROFILE_FUNCTION();

		if (auto spec = framebuffer->getSpec();
			spec.width != viewportSize.x || spec.height != viewportSize.y) {
			framebuffer->resize(viewportSize);
			camera.setViewPortSize(viewportSize);
		}

		Renderer2D::resetStats();

		camera.onUpdate(ts);

		framebuffer->bind();

		RenderCommand::setClearColor({0.1, 0.1, 0.1, 1.0});
		RenderCommand::clear();
		framebuffer->clearAttachment(1, -1);

		activeScene->onUpdateEditor(ts, camera);

		auto&& [mx, my] = ImGui::GetMousePos();
		mx -= viewportBounds[0].x;
		my -= viewportBounds[0].y;
		my = viewportSize.y - my;

		auto viewportSize = viewportBounds[1] - viewportBounds[0];

		if (mx >= 0.0 && my >= 0 && mx < viewportSize.x && my < viewportSize.y) {
			auto mouseX	   = static_cast<int>(mx);
			auto mouseY	   = static_cast<int>(my);
			int	 pixelData = framebuffer->readPixel(1, mouseX, mouseY);
			hoveredEntity  = pixelData == -1 ? Entity{} : Entity(static_cast<entt::entity>(pixelData), activeScene.get());
		}

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
			auto viewportMin	= ImGui::GetWindowContentRegionMin();
			auto viewportMax	= ImGui::GetWindowContentRegionMax();
			auto viewportOffset = ImGui::GetWindowPos();
			viewportBounds[0]	= {viewportMin.x + viewportOffset.x, viewportMin.y + viewportOffset.y};
			viewportBounds[1]	= {viewportMax.x + viewportOffset.x, viewportMax.y + viewportOffset.y};

			viewportFocused = ImGui::IsWindowFocused();
			viewportHovered = ImGui::IsWindowHovered();
			Application::getApplication().getImGuiLayer().blockEvents(!viewportFocused && !viewportHovered);

			auto avail	 = ImGui::GetContentRegionAvail();
			viewportSize = {avail.x, avail.y};

			auto  textureId = framebuffer->getColorAttachmentID();
			void* texId		= static_cast<uint8_t*>(0) + textureId;
			ImGui::Image(texId, {viewportSize.x, viewportSize.y}, {0, 1}, {1, 0});


			//Gizmo stuff
			auto selected = sceneHierarchyPanel.selectedEntity();
			if (selected && gizmoType != -1) {
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();


				ImGuizmo::SetRect(viewportBounds[0].x,
								  viewportBounds[0].y,
								  viewportBounds[1].x - viewportBounds[0].x,
								  viewportBounds[1].y - viewportBounds[0].y);

				//dynamic Camera
				//auto   cameraEntity	 = activeScene->getPrimaryCamera();
				//auto&& cam			 = cameraEntity.get<CameraComponent>().camera;
				//auto&& camProjection = cam.projection();
				//auto&& camView		 = glm::inverse(cameraEntity.get<TransformComponent>().transform());

				//Editor camera
				auto&& camProjection = camera.projection();
				auto&& camView		 = camera.view();

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
		camera.onEvent(e);
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<KeyPressedEvent>(RE_BIND_EVENT_FN(EditorLayer::onKeyPressed));
		dispatcher.dispatch<MouseButtonPressedEvent>(RE_BIND_EVENT_FN(EditorLayer::onMouseButtonPressed));
	}

	bool EditorLayer::onKeyPressed(KeyPressedEvent& e) {
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
				if (!ImGuizmo::IsUsing()) {
					gizmoType = ImGuizmo::OPERATION::TRANSLATE;
				}
				break;
			}
			case KeyCode::E: {
				if (!ImGuizmo::IsUsing()) {
					gizmoType = ImGuizmo::OPERATION::ROTATE;
				}
				break;
			}
			case KeyCode::R: {
				if (!ImGuizmo::IsUsing()) {
					gizmoType = ImGuizmo::OPERATION::SCALE;
				}
				break;
			}
		}
		return true;
	}
	static bool isInsideViewport(bool isHovered) {
		return !ImGuizmo::IsOver() && !Input::isKeyPressed(KeyCode::LEFT_ALT) && isHovered;
	}

	bool EditorLayer::onMouseButtonPressed(MouseButtonPressedEvent& e) {
		if (!isInsideViewport(viewportHovered)) {
			return false;
		}
		if (e.getMouseButton() == MouseCode::BUTTON_LEFT) {
			if (hoveredEntity) {
				RE_CORE_TRACE("Selected entity {0}", hoveredEntity.get<TagComponent>().tag);
			}
			sceneHierarchyPanel.setSelectedEntity(hoveredEntity);
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