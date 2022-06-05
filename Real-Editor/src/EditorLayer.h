#pragma once

#include "Real.h"

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
		OrthographicCameraController controller;

		Ref<VertexArray> squareVA;
		Ref<Shader>      flatColorShader;
		Ref<Texture2D>   checkboardTexture;

		Ref<Texture2D>    spriteSheet;
		Ref<SubTexture2D> barrelSprite;
		Ref<Framebuffer>  framebuffer;

		glm::vec4 squareColor = {0.2f, 0.3f, 0.4f, 1.0f};

		glm::vec2 viewportSize = {0.0f, 0.0f};
	};
}