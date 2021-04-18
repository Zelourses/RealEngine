#pragma once

#include "RealEngine/Core.h"

namespace RealEngine {
	class RE_API Input {
	public:
		static bool isKeyPressed(int keyCode) { return inputInstance->isKeyPressedImpl(keyCode); }

		static bool isMouseButtonPressed(int button) { return inputInstance->isMouseButtonPressedImpl(button); }
		static std::pair<float,float> getMousePosition() { return inputInstance->getMousePositionImpl(); }
		static float getMouseX() { return inputInstance->getMouseXImpl(); }
		static float getMouseY() { return inputInstance->getMouseYImpl(); }

	protected:
		virtual bool isKeyPressedImpl(int keyCode) = 0;
		virtual bool isMouseButtonPressedImpl(int button ) = 0;
		virtual std::pair<float, float> getMousePositionImpl() = 0;
		virtual float getMouseXImpl() = 0;
		virtual float getMouseYImpl() = 0;
		
	private:
		static Input* inputInstance;
	};
}