#pragma once

#include "real/core/Core.h"
#include "real/scene/Scene.h"
#include "real/scene/Entity.h"

namespace Real {
	class SceneHierarchyPanel {
	public:
		SceneHierarchyPanel();
		SceneHierarchyPanel(Ref<Scene>& sceneContext);

		void setContext(Ref<Scene>& sceneContext);
		void onImGUIRender();

		Entity selectedEntity() const;

	private:
		void drawEntityNode(Entity entity);
		void drawComponents(Entity entity);

		Entity	   selectedContext;
		Ref<Scene> context;
	};
}
