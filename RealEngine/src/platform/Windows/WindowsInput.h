#pragma once

#include "RealEngine/Input.h"

namespace RealEngine {
	class WindowsInput : public Input {
		
	protected:
		bool isKeyPressedImpl(int keyCode) override;
		
		bool isMouseButtonPressedImpl(int button) override;
		std::pair<float, float> getMousePositionImpl() override;
		float getMouseXImpl() override;
		float getMouseYImpl() override;
	};
}
