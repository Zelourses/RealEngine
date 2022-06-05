#include "EditorLayer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <platform/OpenGL/OpenGLShader.h>

namespace Real {

	EditorLayer::EditorLayer()
	    : Layer("Sandbox2D"), controller(1280.0f / 720.0f) {}

	void EditorLayer::onAttach() {
		checkboardTexture = Texture2D::create("assets/textures/Checkerboard.png");
		spriteSheet       = Texture2D::create("assets/textures/RPGpack_sheet_2X.png");
		barrelSprite      = SubTexture2D::createFromCoods(spriteSheet, {8, 2}, {128, 128});

		FramebufferSpecification spec = {
		    .width  = 1280,
		    .height = 720};
		framebuffer = Framebuffer::create(spec);
	}

	void EditorLayer::onDetach() {}

	void EditorLayer::onUpdate(Real::Timestep ts) {
		RE_PROFILE_FUNCTION();

		Renderer2D::resetStats();

		controller.onUpdate(ts);

		framebuffer->bind();

		RenderCommand::setClearColor({0.1, 0.1, 0.1, 1.0});
		RenderCommand::clear();

		Renderer2D::beginScene(controller.getCamera());
		Renderer2D::drawQuad({0.0, 0.0, 0.0}, {0.5, 0.5}, barrelSprite);
		Renderer2D::endScene();

		Renderer2D::beginScene(controller.getCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f) {
			for (float x = -5.0f; x < 5.0f; x += 0.5f) {
				glm::vec4 color = {(x + 5.0f) / 10.0f, 0.4, (y + 5.0f) / 10.0f, 1.0};
				Renderer2D::drawQuad({x, y, -0.1f}, {0.45f, 0.45f}, color);
			}
		}
		Renderer2D::endScene();

		framebuffer->unbind();
	}

	void EditorLayer::onImGUIRender() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		ImGui::PopStyleVar();

		auto stats = Real::Renderer2D::getStats();
		ImGui::Begin("Settings");
		ImGui::Text("Renderer2D stats:");
		ImGui::Text("Draw calls: %d", stats.drawCalls);
		ImGui::Text("Quads: %d", stats.quadCount);
		ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
		ImGui::Text("Indices: %d", stats.getTotalIndexCount());
		ImGui::ColorEdit4("label", glm::value_ptr(squareColor));
		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");
		auto res = ImGui::GetContentRegionAvail();
		if (viewportSize != *(glm::vec2*)&res) {// oops...

			viewportSize = {res.x, res.y};
			framebuffer->resize(viewportSize);
			controller.resize(viewportSize);
		}
		auto  textureId = framebuffer->getColorAttachmentID();
		void* texId     = static_cast<uint8_t*>(0) + textureId;
		ImGui::Image(texId, {viewportSize.x, viewportSize.y}, {0, 1}, {1, 0});
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorLayer::onEvent(Real::Event& e) {
		controller.onEvent(e);
	}
}