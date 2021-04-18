#pragma once

#include "repch.h"

#include "RealEngine/Core.h"
#include "RealEngine/Events/Event.h"


namespace RealEngine {

	struct WindowProps {
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProps(const std::string& title = "Real Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			:title(title), width(width), height(height) {}
		
	};

	// Interface representing a window on desktop system
	class RE_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void onUpdate() = 0;

		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;

		//Window attributes
		virtual void setEventCallback(const EventCallbackFn& callback) = 0;
		virtual void setVsync(bool enabled) = 0;
		virtual bool isVsync() const = 0;

		virtual void* getNativeWindow() const = 0;

		static Window* create(const WindowProps& props = WindowProps());
	};
}
