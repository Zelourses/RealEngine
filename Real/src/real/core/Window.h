#pragma once

#include "repch.h"

#include "Real/Core/Core.h"
#include "Real/Events/Event.h"


namespace Real {

	struct WindowProps {
		std::string	 title	= "Real Application";
		unsigned int width	= 1280;
		unsigned int height = 720;
	};

	// Interface representing a window on desktop system
	class Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void onUpdate() = 0;

		virtual unsigned int getWidth() const  = 0;
		virtual unsigned int getHeight() const = 0;

		// Window attributes
		virtual void setEventCallback(const EventCallbackFn& callback) = 0;
		virtual void setVsync(bool enabled)							   = 0;
		virtual bool isVsync() const								   = 0;

		virtual void* getNativeWindow() const = 0;

		static Window* create(const WindowProps& props);

		static float HighDPIWindowScaleFactor;
	};
}
