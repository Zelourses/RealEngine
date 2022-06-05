#pragma once

#include "Real/Core/Core.h"

namespace Real {
	class Input {
	public:
		static bool isKeyPressed(int keyCode);

		static bool                    isMouseButtonPressed(int button);
		static std::pair<float, float> getMousePosition();
		static float                   getMouseX();
		static float                   getMouseY();
	};
}