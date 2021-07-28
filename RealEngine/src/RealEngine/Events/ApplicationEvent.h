#pragma once

#include "Event.h"


namespace RealEngine {
	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: windowWidth(width), windowHeight(height){}

		inline unsigned int getWidth() const { return windowWidth; }
		inline unsigned int getHeight() const { return windowHeight; }

		std::string toString() const override {
			std::stringstream stream;
			stream << "WindowResizeEvent: " << windowWidth << ", " << windowHeight;
			return stream.str();
		}
		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		unsigned int windowWidth, windowHeight;
	};

	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent(){}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppTickEvent : public Event {
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent : public Event {
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent : public Event {
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	
}
