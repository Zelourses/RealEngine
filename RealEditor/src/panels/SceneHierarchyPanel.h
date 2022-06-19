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

	private:
		void	   drawEntityNode(Entity entity);
		Entity	   selectedContext;
		Ref<Scene> context;
	};
}
