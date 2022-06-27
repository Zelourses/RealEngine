#pragma once

#include "Real.h"
#include "panels/SceneHierarchyPanel.h"

namespace Real {
	class EditorLayer : public Layer {
	public:
		EditorLayer();

		void onAttach() override;
		void onDetach() override;

		void onUpdate(Timestep ts) override;
		void onImGUIRender() override;
		void onEvent(Event& e) override;


		~EditorLayer() = default;

	private:
		bool onKeyPressed(KeyPressedEvent e);

		void newScene();
		void openScene();
		void saveSceneAs();

		Ref<VertexArray> squareVA;

		Ref<Framebuffer> framebuffer;

		Ref<Scene> activeScene;

		EditorCamera camera;

		glm::vec2 viewportSize = {1.0f, 1.0f};

		int gizmoType = -1;

		SceneHierarchyPanel sceneHierarchyPanel;
	};
}