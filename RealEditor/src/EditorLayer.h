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


		OrthographicCameraController controller;

		Ref<VertexArray> squareVA;
		Ref<Shader>      flatColorShader;
		Ref<Texture2D>   checkboardTexture;

		Ref<Texture2D>    spriteSheet;
		Ref<SubTexture2D> barrelSprite;
		Ref<Framebuffer>  framebuffer;

		Ref<Scene> activeScene;
		
		Entity squareEntity;
		Entity cameraEntity;

		glm::vec2 viewportSize = {0.0f, 0.0f};

		SceneHierarchyPanel sceneHierarchyPanel;
	};
}