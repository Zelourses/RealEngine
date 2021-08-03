#pragma once

#include "Event.h"


namespace Real {
	class MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float x, float y)
			:mouseX(x),mouseY(y) {}

		inline float getX() const { return mouseX; }
		inline float getY() const {	return mouseY; }

		std::string toString() const override {
			std::stringstream stream;
			stream << "MouseMovedEvent: " << mouseX << "," << mouseY;
			return stream.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
	private:
		float mouseX, mouseY;
	};
	class MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: offsetX(xOffset), offsetY(yOffset){}
		inline float getOffsetX() const { return offsetX; }
		inline float getOffsetY() const { return offsetY; }

		std::string toString() const override {
			std::stringstream stream;
			stream << "MouseScrolledEvent: " << offsetX << "," << offsetY;
			return stream.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
	private:
		float offsetX, offsetY;
	};


	class MouseButtonEvent : public Event {
	public:
		inline int getMouseButton() const { return mouseButton; }

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
	protected:
		MouseButtonEvent(int button)
			:mouseButton(button) {}
		int mouseButton;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int button)
			:MouseButtonEvent(button) {}
		
		std::string toString() const override {
			std::stringstream stream;
			stream << "MouseButtonPressedEvent: " << mouseButton;
			return stream.str();
		}
		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int button)
			:MouseButtonEvent(button) {}

		std::string toString() const override {
			std::stringstream stream;
			stream << "MouseButtonReleasedEvent: " << mouseButton;
			return stream.str();
		}
		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}


