#pragma once
#include "RealEngine/Window.h"

#include "GLFW/glfw3.h"

namespace RealEngine {
	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow();

		void onUpdate() override;

		unsigned getWidth() const override;
		unsigned getHeight() const override;

		void setEventCallback(const EventCallbackFn& callback) override { windowData.eventCallback = callback; }

		void setVsync(bool enabled) override;
		bool isVsync() const override;

	private:
		virtual void init(const WindowProps& prop);
		virtual void shutdown();

		GLFWwindow* window;
		
		struct WindowData {
			std::string title;
			unsigned int width, height;
			bool Vsync;

			EventCallbackFn eventCallback;
		};

		WindowData windowData;
	};
}

