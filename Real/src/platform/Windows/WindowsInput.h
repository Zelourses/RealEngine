#pragma once

#include "Real/Core/Input.h"

namespace Real {
	class WindowsInput : public Input {
		
	protected:
		bool isKeyPressedImpl(int keyCode) override;
		
		bool isMouseButtonPressedImpl(int button) override;
		std::pair<float, float> getMousePositionImpl() override;
		float getMouseXImpl() override;
		float getMouseYImpl() override;
	};
}
