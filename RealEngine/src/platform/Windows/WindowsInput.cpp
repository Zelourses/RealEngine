#include "repch.h"
#include "WindowsInput.h"

#include "RealEngine/Application.h"
#include "GLFW/glfw3.h"

namespace RealEngine {

	Input* Input::inputInstance = new WindowsInput();

	bool WindowsInput::isKeyPressedImpl(int keyCode) {
		auto* const window = static_cast<GLFWwindow*>(Application::getApplication().getWindow().getNativeWindow());
		const auto state = glfwGetKey(window, keyCode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::isMouseButtonPressedImpl(int button) {
		auto* const window = static_cast<GLFWwindow*>(Application::getApplication().getWindow().getNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::getMousePositionImpl() {
		auto* const window = static_cast<GLFWwindow*>(Application::getApplication().getWindow().getNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		return {static_cast<float>(xPos),static_cast<float>(yPos)};
	}

	float WindowsInput::getMouseXImpl() {
		auto [x, y] = getMousePositionImpl();
		return x;
		
	}
	float WindowsInput::getMouseYImpl() {
		auto [x, y] = getMousePositionImpl();
		return y;
	}
}
