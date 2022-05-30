#pragma once

#include "Real.h"

class Sandbox2D : public Real::Layer {
public:
	Sandbox2D();
	~Sandbox2D() = default;


	void onAttach() override;
	void onDetach() override;

	void onUpdate(Real::Timestep ts) override;
	void onImGUIRender() override;
	void onEvent(Real::Event& e) override;

private:
	Real::OrthographicCameraController controller;

	Real::Ref<Real::VertexArray> squareVA;
	Real::Ref<Real::Shader> flatColorShader;
	Real::Ref<Real::Texture2D> checkboardTexture;
	
	Real::Ref<Real::Texture2D> spriteSheet;
	Real::Ref<Real::SubTexture2D> barrelSprite;

	glm::vec4 squareColor = { 0.2f,0.3f,0.4f,1.0f };

};
