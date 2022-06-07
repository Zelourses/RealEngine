#pragma once
#include "Real/Core/Window.h"

#include "GLFW/glfw3.h"

#include "Real/Renderer/GraphicsContext.h"


namespace Real {
	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow();

		void onUpdate() override;

		unsigned int getWidth() const override;
		unsigned int getHeight() const override;

		void setEventCallback(const EventCallbackFn& callback) override { windowData.eventCallback = callback; }

		void* getNativeWindow() const override { return window; }

		void setVsync(bool enabled) override;
		bool isVsync() const override;

	private:
		GLFWwindow* window;
		GraphicsContext* context;

		virtual void init(const WindowProps& prop);
		virtual void shutdown();
		
		struct WindowData {
			std::string title;
			unsigned int width, height;
			bool Vsync;

			EventCallbackFn eventCallback;
		};

		WindowData windowData;
	};
}

