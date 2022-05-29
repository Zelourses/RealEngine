#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <platform/OpenGL/OpenGLShader.h>


Sandbox2D::Sandbox2D() 
	: Layer("Sandbox2D"), controller(1280.0f / 720.0f) {}

void Sandbox2D::onAttach()
{
	checkboardTexture = Real::Texture2D::create("assets/textures/Checkerboard.png");
}

void Sandbox2D::onDetach() {}

void Sandbox2D::onUpdate(Real::Timestep ts) {

	RE_PROFILE_FUNCTION();

	Real::Renderer2D::resetStats();

	controller.onUpdate(ts);

	Real::RenderCommand::setClearColor({0.1,0.1,0.1,1.0});
	Real::RenderCommand::clear();

	Real::Renderer2D::beginScene(controller.getCamera());

	Real::Renderer2D::drawQuad({-1.0,0.0,0.0},{0.8, 0.8},{ 0.8f, 0.2f, 0.3f, 1.0f });
	Real::Renderer2D::drawQuad({0.5,-0.5,0.0},{0.5,0.75},{0.1,0.2,0.8,1.0});
	Real::Renderer2D::drawQuad({0.0,0.0,-0.1},{0.5, 0.5},checkboardTexture);
	Real::Renderer2D::drawRotatedQuad({ 0.0,0.0,-0.2 }, { 1.0,1.0 }, glm::radians(45.0f), checkboardTexture);

	Real::Renderer2D::endScene();

	Real::Renderer2D::beginScene(controller.getCamera());
	for (float y = -5.0f; y < 5.0f; y+=0.5f) {
		for (float x = -5.0f; x < 5.0f; x += 0.5f) {
			glm::vec4 color = {(x + 5.0f) / 10.0, 0.4, (y + 5.0f) / 10.0, 1.0};
			Real::Renderer2D::drawQuad({x, y, 0.0f}, {0.45f, 0.45f}, color);
		}
	}
	Real::Renderer2D::endScene();
}

void Sandbox2D::onImGUIRender() {

	auto stats = Real::Renderer2D::getStats();
	ImGui::Begin("Settings");
	ImGui::Text("Renderer2D stats:");
	ImGui::Text("Draw calls: %d",stats.drawCalls);
	ImGui::Text("Quads: %d",stats.quadCount);
	ImGui::Text("Vertices: %d",stats.getTotalVertexCount());
	ImGui::Text("Indices: %d",stats.getTotalIndexCount());
	//ImGui::ColorEdit4("SquareColor",glm::value_ptr(squareColor));
	ImGui::End();
}

void Sandbox2D::onEvent(Real::Event& e) {
	controller.onEvent(e);
}
