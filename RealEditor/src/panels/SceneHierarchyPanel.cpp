#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>

#include "real/scene/Components.h"

namespace Real {
	SceneHierarchyPanel::SceneHierarchyPanel() {}
	SceneHierarchyPanel::SceneHierarchyPanel(Ref<Scene>& sceneContext) {
		setContext(sceneContext);
	}
	void SceneHierarchyPanel::setContext(Ref<Scene>& sceneContext) {
		context = sceneContext;
	}
	void SceneHierarchyPanel::onImGUIRender() {
		ImGui::Begin("Scene hierarchy");

		context->registry.each([this](auto&& entityId) {
			Entity ent{entityId, context.get()};

			drawEntityNode(ent);
		});

		ImGui::End();
	}
	void SceneHierarchyPanel::drawEntityNode(Entity entity) {
		auto&& tc	= entity.get<TagComponent>().tag;
		auto   id	= entity.id();
		void*  data = static_cast<int*>(0) + id;

		auto selectedFlag = selectedContext == entity
							  ? ImGuiTreeNodeFlags_Selected
							  : 0;
		auto flags		  = ImGuiTreeNodeFlags_OpenOnArrow
				   | selectedFlag;
		bool opened = ImGui::TreeNodeEx(data, flags, "%s", tc.c_str());

		//FIXME: change to callback
		if (ImGui::IsItemClicked()) {
			selectedContext = entity;
		}
		if (opened) {
			ImGui::TreePop();
		}
	}
}