#define _CRT_SECURE_NO_WARNINGS
#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

#include "real/scene/Components.h"

namespace Real {

	static void drawVec3Control(std::string_view label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100) {
		auto&& io = ImGui::GetIO();
		//FIXME: It will broke after I add other font than Bold and regular;

		auto&& boldFont = io.Fonts->Fonts[0];
		ImGui::PushID(label.data());
		//From imgui internal functions
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.data());

		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2{0, 0});

		float  lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize)) {
			values.x = resetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0, 0, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.8f, 0.3f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize)) {
			values.y = resetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0, 0, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize)) {
			values.z = resetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0, 0, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);
		ImGui::PopID();
	}

	SceneHierarchyPanel::SceneHierarchyPanel() {}
	SceneHierarchyPanel::SceneHierarchyPanel(Ref<Scene>& sceneContext) {
		setContext(sceneContext);
	}
	void SceneHierarchyPanel::setContext(Ref<Scene>& sceneContext) {
		context = sceneContext;
		selectedContext = {};
	}
	void SceneHierarchyPanel::onImGUIRender() {
		ImGui::Begin("Scene hierarchy");

		context->registry.each([this](auto&& entityId) {
			Entity ent{entityId, context.get()};

			drawEntityNode(ent);
		});

		/// If I want to desselect entities
		/*if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
			selectedContext = {};
		}*/

		//right-click on empty space
		if (ImGui::BeginPopupContextWindow(0, 1, false)) {
			if (ImGui::MenuItem("Create Entity")) {
				context->createEntity();
			}
			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");

		if (selectedContext) {
			drawComponents(selectedContext);
		}

		ImGui::End();
	}
	void SceneHierarchyPanel::drawEntityNode(Entity entity) {
		auto&& tc	= entity.get<TagComponent>().tag;
		auto   id	= entity.id();
		void*  data = static_cast<int*>(0) + id;

		auto selectedFlag = selectedContext == entity
							  ? ImGuiTreeNodeFlags_Selected
							  : 0;
		auto flags		  = ImGuiTreeNodeFlags_OpenOnArrow | selectedFlag | ImGuiTreeNodeFlags_SpanAvailWidth;

		bool opened = ImGui::TreeNodeEx(data, flags, "%s", tc.c_str());

		//FIXME: change to callback
		if (ImGui::IsItemClicked()) {
			selectedContext = entity;
		}
		bool isDeleting = false;
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Delete Entity")) {
				isDeleting = true;
			}
			ImGui::EndPopup();
		}

		if (opened) {
			ImGui::TreePop();
		}

		if (isDeleting) {
			context->destoryEntity(entity);
			if (selectedContext == entity)
				selectedContext = {};
		}
	}
	template <typename ComponentType, bool removable = true, typename UIFunction>
	static void drawComponent(std::string_view name, Entity entity, UIFunction&& function) {
		const auto&& treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen
								   | ImGuiTreeNodeFlags_AllowItemOverlap
								   | ImGuiTreeNodeFlags_SpanAvailWidth
								   | ImGuiTreeNodeFlags_Framed
								   | ImGuiTreeNodeFlags_FramePadding;

		if (entity.allOf<ComponentType>()) {
			auto contentRegionAvail = ImGui::GetContentRegionAvail().x;
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {4, 4});
			float lineHeight = GImGui->Font->FontSize
							 + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();

			const auto open = ImGui::TreeNodeEx(name.data(), treeNodeFlags);
			ImGui::PopStyleVar();

			if constexpr (removable) {
				ImGui::SameLine(contentRegionAvail - lineHeight * 0.5f);
				if (ImGui::Button("+", {lineHeight, lineHeight})) {
					ImGui::OpenPopup("ComponentSettings");
				}
			}

			bool isRemoving = false;

			if constexpr (removable) {
				if (ImGui::BeginPopup("ComponentSettings")) {
					if (ImGui::MenuItem("Remove component")) {
						isRemoving = true;
					}
					ImGui::EndPopup();
				}
			}

			if (open) {
				auto&& component = entity.get<ComponentType>();
				function(component);
				ImGui::TreePop();
			}

			if constexpr (removable) {
				if (isRemoving)
					entity.remove<ComponentType>();
			}
		}
	}
	void SceneHierarchyPanel::drawComponents(Entity entity) {
		
		if (entity.allOf<TagComponent>()) {
			auto&& tc = entity.get<TagComponent>().tag;
			//FIXME: wide chars will broke this 100%
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));

			std::strncpy(buffer, tc.c_str(),sizeof(buffer));
#undef _CRT_SECURE_NO_WARNINGS

			if (ImGui::InputText("##Tag", buffer, sizeof(buffer) - 1 /*null-terminator*/)) {
				tc = std::move(std::string(buffer));
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		#if 1
		if (ImGui::Button("Add compponent")) {
			ImGui::OpenPopup("AddComponent");
		}

		if (ImGui::BeginPopup("AddComponent")) {
			if (ImGui::MenuItem("Camera") && !selectedContext.allOf<CameraComponent>()) {
				selectedContext.add<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Sprite Renderer") && !selectedContext.allOf<SpriteRendererComponent>()) {
				selectedContext.add<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		#endif
		ImGui::PopItemWidth();

		drawComponent<TransformComponent, false>("Transform", entity, [](auto&& tc) {
			drawVec3Control("Position", tc.translation);
			auto rotation = glm::degrees(tc.rotaion);
			drawVec3Control("Rotation", rotation);
			tc.rotaion = glm::radians(rotation);
			drawVec3Control("Scale", tc.scale, 1.0);
		});

		drawComponent<CameraComponent>("Camera", entity, [](auto&& cameraComponent) {
			auto&& camera = cameraComponent.camera;

			ImGui::Checkbox("Primary", &cameraComponent.primary);

			const char* projectionTypeString[] = {"Perspective", "Orthograpchic"};
			const char* current				   = projectionTypeString[static_cast<unsigned>(camera.projectionType())];
			if (ImGui::BeginCombo("Projection", current)) {
				for (int i = 0; i < 2; i++) {
					bool isSelected = current == projectionTypeString[i];
					if (ImGui::Selectable(projectionTypeString[i], isSelected)) {
						current = projectionTypeString[i];
						camera.setProjectionType(static_cast<SceneCamera::ProjectionType>(i));
					}

					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();
			}

			if (camera.projectionType() == SceneCamera::ProjectionType::Orthographic) {
				float val = camera.orthographicSize();
				if (ImGui::DragFloat("Size", &val)) {
					camera.setOrthographicSize(val);
				}

				val = camera.orthographicNear();
				if (ImGui::DragFloat("Near", &val)) {
					camera.setOrthographicNear(val);
				}

				val = camera.orthographicFar();
				if (ImGui::DragFloat("Far", &val)) {
					camera.setOrthographicFar(val);
				}

				ImGui::Checkbox("Fixed aspect ratio", &cameraComponent.fixedAspectRatio);
			}
			if (camera.projectionType() == SceneCamera::ProjectionType::Perspective) {
				float val = glm::degrees(camera.perspectiveVerticalFOV());
				if (ImGui::DragFloat("Vertical FOV", &val)) {
					camera.setPerspectiveVerticalFOV(glm::radians(val));
				}

				val = camera.perspectiveNear();
				if (ImGui::DragFloat("Near", &val)) {
					camera.setPerspectiveNear(val);
				}

				val = camera.perspectiveFar();
				if (ImGui::DragFloat("Far", &val)) {
					camera.setPerspectiveFar(val);
				}
			}
		});

		drawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto&& component) {
			ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
		});
	}
}