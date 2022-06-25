#include "repch.h"

#include "OrthographicCameraController.h"
#include "Real/Core/Input.h"

#include "Real/Core/KeyCode.h"

namespace Real {

	OrthographicCameraController::OrthographicCameraController(float camAspectRatio, bool camRotation)
	    : aspectRatio(camAspectRatio), camera(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel),
	      rotation(camRotation) {
	}
	void OrthographicCameraController::onUpdate(Timestep ts) {
		if (Input::isKeyPressed(KeyCode::A)) {
			cameraPosition.x -= cameraTranslationSpeed * ts;
		} else if (Input::isKeyPressed(KeyCode::D)) {
			cameraPosition.x += cameraTranslationSpeed * ts;
		}
		if (Input::isKeyPressed(KeyCode::W)) {
			cameraPosition.y += cameraTranslationSpeed * ts;
		} else if (Input::isKeyPressed(KeyCode::S)) {
			cameraPosition.y -= cameraTranslationSpeed * ts;
		}

		camera.setPosition(cameraPosition);

		if (rotation) {
			if (Input::isKeyPressed(KeyCode::Q)) {
				cameraRotation += cameraRotationSpeed * ts;
			} else if (Input::isKeyPressed(KeyCode::E)) {
				cameraRotation -= cameraRotationSpeed * ts;
			}
			camera.setRotation(cameraRotation);
		}

		cameraTranslationSpeed = zoomLevel;
	}

	void OrthographicCameraController::resize(const glm::vec2& windowSize) {
		aspectRatio = windowSize.x / windowSize.y;
		camera.setProjection(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
	}


	void OrthographicCameraController::onEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(RE_BIND_EVENT_FN(OrthographicCameraController::onMouseScrolled));
		dispatcher.dispatch<WindowResizeEvent>(RE_BIND_EVENT_FN(OrthographicCameraController::onWindowResized));
	}
	bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e) {
		zoomLevel -= e.getOffsetY() * cameraZoomMultiplier;
		zoomLevel = std::max(zoomLevel, 0.1f);

		camera.setProjection(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);

		return false;
	}
	bool OrthographicCameraController::onWindowResized(WindowResizeEvent& e) {
		resize({e.getWidth(), e.getHeight()});

		return false;
	}
}
