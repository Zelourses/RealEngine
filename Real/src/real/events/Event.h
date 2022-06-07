#pragma once
#include "repch.h"

#include "Real/Core/Core.h"


namespace Real {
	/*
	 * Events in engine are currently blocking type.
	 * This means that when event occurs, it immediately gets dispatched and must be resolved right there.
	 * On that moment nothing like EventBuffer with event "resolving" part will be implement.
	 */
	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory {
		None                     = 0,
		EventCategoryApplication = BIT_SHIFT(0),
		EventCategoryInput       = BIT_SHIFT(1),
		EventCategoryKeyboard    = BIT_SHIFT(2),
		EventCategoryMouse       = BIT_SHIFT(3),
		EventCategoryMouseButton = BIT_SHIFT(4)
	};

#define EVENT_CLASS_TYPE(type)                                                    \
	static EventType    getStaticType() { return EventType::##type; }             \
	virtual EventType   getEventType() const override { return getStaticType(); } \
	virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
	virtual int getCategoryFlags() const override { return category; }

	class Event {
	public:
		virtual EventType   getEventType() const     = 0;
		virtual const char* getName() const          = 0;
		virtual int         getCategoryFlags() const = 0;
		virtual std::string toString() const { return getName(); }

		inline bool isInCategory(EventCategory category) {
			return getCategoryFlags() & category;
		}


		bool handled = false;

		virtual ~Event() = default;
	};

	class EventDispatcher {
		template <typename T>
		using EventFun = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event)
		    : event(event) {}
		template <typename T>
		bool dispatch(EventFun<T> func) {
			if (event.getEventType() == T::getStaticType()) {
				event.handled = func(*static_cast<T*>(&event));
				return true;
			}
			return false;
		}

	private:
		Event& event;
	};

	inline std::ostream& operator<<(std::ostream& stream, const Event& event) {
		return stream << event.toString();
	}
}
