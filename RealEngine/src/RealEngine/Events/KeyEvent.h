#pragma once

#include "Event.h"

namespace RealEngine {
	class KeyEvent : public Event {
	public:
		inline int getKeyCode() const { return keyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		int keyCode;
		KeyEvent(int keyCode) : keyCode(keyCode) { }
		
	};

	class KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(int keyCode, int repeatCount)
			: KeyEvent(keyCode), repeatCount(repeatCount) {}

		inline int getRepeatCount() const { return repeatCount; }

		std::string toString() const override {
			std::stringstream stream;
			stream << "KeyPressedEvent: " << keyCode << " (" << repeatCount << " repeats)";
			return stream.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int repeatCount;

	};

	class KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keyCode)
			: KeyEvent(keyCode) {}

		std::string toString() const override {
			std::stringstream stream;
			stream << "KeyReleasedEvent: " << keyCode;
			return stream.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(int keyCode)
			: KeyEvent(keyCode) {}

		std::string toString() const override {
			std::stringstream stream;
			stream << "KeyTypedEvent: " << keyCode;
			return stream.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)

	};
}