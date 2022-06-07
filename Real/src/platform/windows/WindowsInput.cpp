#include "repch.h"
#include "Real/core/Input.h"

#include "Real/Core/Application.h"
#include "GLFW/glfw3.h"

namespace Real {


	bool Input::isKeyPressed(int keyCode) {
		auto* const window = static_cast<GLFWwindow*>(Application::getApplication().getWindow().getNativeWindow());
		const auto  state  = glfwGetKey(window, keyCode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::isMouseButtonPressed(int button) {
		auto* const window = static_cast<GLFWwindow*>(Application::getApplication().getWindow().getNativeWindow());
		auto        state  = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::getMousePosition() {
		auto* const window = static_cast<GLFWwindow*>(Application::getApplication().getWindow().getNativeWindow());
		double      xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		return {static_cast<float>(xPos), static_cast<float>(yPos)};
	}

	float Input::getMouseX() {
		auto [x, y] = getMousePosition();
		return x;
	}
	float Input::getMouseY() {
		auto [x, y] = getMousePosition();
		return y;
	}
}
