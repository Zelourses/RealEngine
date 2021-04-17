#pragma once

#include "RealEngine/Layer.h"
#include "RealEngine/Events/ApplicationEvent.h"
#include "RealEngine/Events/KeyEvent.h"
#include "RealEngine/Events/MouseEvent.h"

namespace RealEngine {
	class RE_API ImGUILayer : public Layer{
	public:
		ImGUILayer();
		~ImGUILayer();

		void onAttach() override;
		void onDetach() override;

		void onUpdate() override;
		void onEvent(Event& event) override;

	private:
		bool onMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool onMouseScrolledEvent(MouseScrolledEvent& e);
		bool onMouseMovedEvent(MouseMovedEvent& e);
		bool onKeyPressedEvent(KeyPressedEvent& e);
		bool onKeyReleasedEvent(KeyReleasedEvent& e);
		bool onKeyTypedEvent(KeyTypedEvent& e);
		bool onWindowResizeEvent(WindowResizeEvent& e);
		
		
		float frameTime = 0.0f;
	};
}

