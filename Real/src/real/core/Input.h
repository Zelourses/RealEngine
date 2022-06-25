#pragma once

#include "Real/Core/Core.h"
#include "real/core/KeyCode.h"
#include "real/core/MouseCodes.h"

namespace Real {
	class Input {
	public:
		static bool isKeyPressed(KeyCode keyCode);

		static bool					   isMouseButtonPressed(MouseCodes button);
		static std::pair<float, float> getMousePosition();
		static float                   getMouseX();
		static float                   getMouseY();
	};
}