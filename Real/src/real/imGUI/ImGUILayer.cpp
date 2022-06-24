#include "repch.h"
#include "ImGUILayer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"


#include "Real/Core/Application.h"

//TEMPORARY THINGS, IT'S A BIG DEPENDENCY PROBLEM
//Nooo my architecture layers abstraction
//TODO: fix this layer dependency
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Real {

	ImGUILayer::ImGUILayer() : Layer("ImGUILayer") {}

	ImGUILayer::~ImGUILayer() {
	}
	void ImGUILayer::onAttach() {
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	// Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		io.Fonts->AddFontFromFileTTF("assets/fonts/JetBrainsMono/JetBrainsMono-Bold.ttf", Window::HighDPIWindowScaleFactor * 16.0f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/JetBrainsMono/JetBrainsMono-Regular.ttf", Window::HighDPIWindowScaleFactor * 16.0f);

		// Setup Dear ImGui style
		ImGui::StyleColorsDark(); // Maybe I did not set up some colors
		setDarkTheme();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding			  = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		Application& app	= Application::getApplication();
		GLFWwindow*	 window = static_cast<GLFWwindow*>(app.getWindow().getNativeWindow());
		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");

	}

	void ImGUILayer::begin() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGUILayer::end() {
		auto&		 io	 = ImGui::GetIO();
		Application& app = Application::getApplication();
		io.DisplaySize	 = ImVec2(static_cast<float>(app.getWindow().getWidth()), static_cast<float>(app.getWindow().getHeight()));

		//Render
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backup_current_window = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_window);
		}
	}

	void ImGUILayer::onDetach() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGUILayer::setDarkTheme() {
		auto&& colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_WindowBg] = {0.1f, 0.105f, 0.11f, 1.0f};

		// Headers
		colors[ImGuiCol_Header]		   = {0.2f, 0.205f, 0.21f, 1.0f};
		colors[ImGuiCol_HeaderHovered] = {0.3f, 0.305f, 0.31f, 1.0f};
		colors[ImGuiCol_HeaderActive]  = {0.15f, 0.1505f, 0.151f, 1.0f};

		// Buttons
		colors[ImGuiCol_Button]		   = {0.2f, 0.205f, 0.21f, 1.0f};
		colors[ImGuiCol_ButtonHovered] = {0.3f, 0.305f, 0.31f, 1.0f};
		colors[ImGuiCol_ButtonActive]  = {0.15f, 0.1505f, 0.151f, 1.0f};

		// Frame BG
		colors[ImGuiCol_FrameBg]		= {0.2f, 0.205f, 0.21f, 1.0f};
		colors[ImGuiCol_FrameBgHovered] = {0.3f, 0.305f, 0.31f, 1.0f};
		colors[ImGuiCol_FrameBgActive]	= {0.15f, 0.1505f, 0.151f, 1.0f};

		// Tabs
		colors[ImGuiCol_Tab]				= {0.15f, 0.1505f, 0.151f, 1.0f};
		colors[ImGuiCol_TabHovered]			= {0.38f, 0.3805f, 0.381f, 1.0f};
		colors[ImGuiCol_TabActive]			= {0.28f, 0.2805f, 0.281f, 1.0f};
		colors[ImGuiCol_TabUnfocused]		= {0.15f, 0.1505f, 0.151f, 1.0f};
		colors[ImGuiCol_TabUnfocusedActive] = {0.2f, 0.205f, 0.21f, 1.0f};

		// Title
		colors[ImGuiCol_TitleBg]		  = {0.15f, 0.1505f, 0.151f, 1.0f};
		colors[ImGuiCol_TitleBgActive]	  = {0.15f, 0.1505f, 0.151f, 1.0f};
		colors[ImGuiCol_TitleBgCollapsed] = {0.15f, 0.1505f, 0.151f, 1.0f};
	}
}
