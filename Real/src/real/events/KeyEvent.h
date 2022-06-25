#pragma once

#include "Event.h"
#include "real/core/KeyCode.h"

namespace Real {
	class KeyEvent : public Event {
	public:
		inline KeyCode getKeyCode() const { return keyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyCode keyCode;
		KeyEvent(KeyCode keyCode) : keyCode(keyCode) { }
		
	};

	class KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(KeyCode keyCode, int repeatCount)
			: KeyEvent(keyCode), repeatCount(repeatCount) {}

		inline int getRepeatCount() const { return repeatCount; }

		std::string toString() const override {
			std::stringstream stream;
			stream << "KeyPressedEvent: " << static_cast<uint16_t>(keyCode) << " (" << repeatCount << " repeats)";
			return stream.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int repeatCount;

	};

	class KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(KeyCode keyCode)
			: KeyEvent(keyCode) {}

		std::string toString() const override {
			std::stringstream stream;
			stream << "KeyReleasedEvent: " << static_cast<uint16_t>(keyCode);
			return stream.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(KeyCode keyCode)
			: KeyEvent(keyCode) {}

		std::string toString() const override {
			std::stringstream stream;
			stream << "KeyTypedEvent: " << static_cast<uint16_t>(keyCode);
			return stream.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)

	};
}