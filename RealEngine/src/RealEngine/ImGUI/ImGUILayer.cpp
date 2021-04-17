#include "repch.h"
#include "ImGUILayer.h"
#include "imgui.h"

#include "platform/OpenGL/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

#include "RealEngine/Application.h"

//TEMPORARY THINGS, IT'S A BIG DEPENDENCY PROBLEM
//Nooo my architecture layers abstraction
//TODO: fix this layer dependency
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace RealEngine {

	ImGUILayer::ImGUILayer() : Layer("ImGUILayer"){}
	
	ImGUILayer::~ImGUILayer() {
		
	}
	void ImGUILayer::onAttach() {
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		
		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;


		//TEMPORARY: glfw_keys must be changed in future
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGUILayer::onDetach() {
		
	}

	void ImGUILayer::onUpdate() {

		auto& io = ImGui::GetIO();
		Application& app = Application::getApplication();
		io.DisplaySize = ImVec2(app.getWindow().getWidth(), app.getWindow().getHeight());

		auto time = static_cast<float>(glfwGetTime());
		io.DeltaTime = frameTime > 0.0 ? (time - frameTime) : (1.0f / 60.0f);
		frameTime = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		
	}
	
	void ImGUILayer::onEvent(Event& event) {
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<MouseButtonPressedEvent>(RE_BIND_EVENT_FN(ImGUILayer::onMouseButtonPressedEvent));
		dispatcher.dispatch<MouseButtonReleasedEvent>(RE_BIND_EVENT_FN(ImGUILayer::onMouseButtonReleasedEvent));
		dispatcher.dispatch<MouseScrolledEvent>(RE_BIND_EVENT_FN(ImGUILayer::onMouseScrolledEvent));
		dispatcher.dispatch<MouseMovedEvent>(RE_BIND_EVENT_FN(ImGUILayer::onMouseMovedEvent));
		dispatcher.dispatch<KeyPressedEvent>(RE_BIND_EVENT_FN(ImGUILayer::onKeyPressedEvent));
		dispatcher.dispatch<KeyReleasedEvent>(RE_BIND_EVENT_FN(ImGUILayer::onKeyReleasedEvent));
		dispatcher.dispatch<KeyTypedEvent>(RE_BIND_EVENT_FN(ImGUILayer::onKeyTypedEvent));
		dispatcher.dispatch<WindowResizeEvent>(RE_BIND_EVENT_FN(ImGUILayer::onWindowResizeEvent));
	}

	bool ImGUILayer::onMouseButtonPressedEvent(MouseButtonPressedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.getMouseButton()] = true;

		//We don't want to consume all events by this debug ImgUI callback, so we return false
		return false;
	}
	bool ImGUILayer::onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.getMouseButton()] = false;

		//We don't want to consume all events by this debug ImgUI callback, so we return false
		return false;
	}
	bool ImGUILayer::onMouseMovedEvent(MouseMovedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.getX(), e.getY());

		//We don't want to consume all events by this debug ImgUI callback, so we return false
		return false;
	}
	bool ImGUILayer::onMouseScrolledEvent(MouseScrolledEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += e.getOffsetX();
		io.MouseWheelH += e.getOffsetY();

		//We don't want to consume all events by this debug ImgUI callback, so we return false
		return false;
	}
	bool ImGUILayer::onKeyPressedEvent(KeyPressedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.getKeyCode()] = true;

		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

		//We don't want to consume all events by this debug ImgUI callback, so we return false
		return false;
	}

	bool ImGUILayer::onKeyTypedEvent(KeyTypedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		int keyCode = e.getKeyCode();
		if (keyCode > 0 && keyCode < 0x10000) {
			io.AddInputCharacter(static_cast<unsigned short>(keyCode));
		}

		//We don't want to consume all events by this debug ImgUI callback, so we return false
		return false;
	}


	bool ImGUILayer::onKeyReleasedEvent(KeyReleasedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.getKeyCode()] = false;

		//We don't want to consume all events by this debug ImgUI callback, so we return false
		return false;
	}
	bool ImGUILayer::onWindowResizeEvent(WindowResizeEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.getWidth(), e.getHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, e.getWidth(), e.getHeight());

		return false;
	}

}
