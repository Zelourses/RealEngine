#include "repch.h"
#include "EditorCamera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#undef GLM_ENABLE_EXPERIMENTAL
#include "real/core/Input.h"

namespace Real {
	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: fov(fov), aspectRatio(aspectRatio), nearClip(nearClip), farClip(farClip),
		  Camera(glm::perspective(fov, aspectRatio, nearClip, farClip)) {
		updateView();
	}

	void EditorCamera::onUpdate(Timestep ts) {
		if (Input::isKeyPressed(KeyCode::LEFT_ALT)) {
			//Maybe rework input?
			const auto [x, y] = Input::getMousePosition();

			const auto mousePos = glm::vec2{x, y};
			auto	   delta	= (mousePos - initialMousePos) * 0.03f;
			initialMousePos		= mousePos;

			if (Input::isMouseButtonPressed(MouseCode::BUTTON_MIDDLE)) {
				mousePan(delta);
			} else if (Input::isMouseButtonPressed(MouseCode::BUTTON_LEFT)) {
				mouseRotate(delta);
			} else if (Input::isMouseButtonPressed(MouseCode::BUTTON_RIGHT)) {
				mouseZoom(delta.y);
			}
		}
		updateView();
	}

	void EditorCamera::onEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(RE_BIND_EVENT_FN(EditorCamera::onMouseScroll));
	}

	glm::mat4 EditorCamera::viewPorjection() const {
		return projectionMatrix * viewMatrix;
	}

	glm::vec3 EditorCamera::upDirection() const {
		return glm::rotate(orientation(),glm::vec3(0.0f,1.0f,0.0f));
	}

	glm::vec3 EditorCamera::rightDirection() const {
		return glm::rotate(orientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::forwardDirection() const {
		return glm::rotate(orientation(), glm::vec3(0.0f, 0.0f, -1.0f /*or -1?*/));
	}

	const glm::vec3& EditorCamera::position() const {
		return camPosition;
	}

	glm::quat EditorCamera::orientation() const {
		return glm::quat(glm::vec3(-camPitch, -camYaw,0.0f));
	}

	float EditorCamera::pitch() const {
		return camPitch;
	}

	float EditorCamera::yaw() const {
		return camYaw;
	}

	void EditorCamera::updateProjection() {
		aspectRatio = viewportWidth / viewportHeight;
		projectionMatrix = glm::perspective(fov,aspectRatio,nearClip,farClip);
	}

	void EditorCamera::updateView() {
		camPosition = calculatePosition();

		auto camOrientation = orientation();
		viewMatrix			= glm::translate(glm::mat4(1.0f), camPosition) * glm::toMat4(camOrientation);
		viewMatrix			= glm::inverse(viewMatrix);
	}

	bool EditorCamera::onMouseScroll(MouseScrolledEvent& e) {
		auto delta = e.getOffsetY() * 0.1f;
		mouseZoom(delta);
		updateView();
		return false;
	}

	void EditorCamera::mousePan(const glm::vec2& delta) {
		const auto speed = panSpeed();
		focalPoint += -rightDirection() * delta.x * speed.x * viewDistance;
		focalPoint += rightDirection() * delta.y * speed.y * viewDistance;
	}

	void EditorCamera::mouseRotate(const glm::vec2& delta) {
		auto yawSign = upDirection().y < 0.0f ? -1.0f : 1.0f;
		camYaw += yawSign * delta.x * rotationSpeed();
		camPitch += delta.y * rotationSpeed();
	}

	void EditorCamera::mouseZoom(float delta) {
		viewDistance -= delta * zoomSpeed();
		if (viewDistance < 1.0f) {
			focalPoint += forwardDirection();
			viewDistance = 1.0f;
		}
	}

	glm::vec3 EditorCamera::calculatePosition() const {
		return focalPoint - forwardDirection() * viewDistance;
	}

	glm::vec2 EditorCamera::panSpeed() const {
		auto x		 = std::min(viewportWidth / 1000.0f, 2.4f);
		auto xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f; // ax^2+bx+c, numbers from desmos

		auto y		 = std::min(viewportHeight / 1000.0f, 2.4f);
		auto yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f; // ax^2+bx+c, numbers from desmos

		return {xFactor, yFactor};
	}

	float EditorCamera::rotationSpeed() const {
		return 0.8f;
	}

	float EditorCamera::zoomSpeed() const {
		auto dist  = viewDistance * 0.2f;
		dist	   = std::max(dist, 0.0f);
		auto speed = dist * dist;
		speed	   = std::min(speed, 100.0f);
		return speed;
	}
}