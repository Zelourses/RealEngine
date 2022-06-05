#pragma once
#include "Real/Renderer/OrthographicCamera.h"

#include "Real/Core/Timestep.h"

#include "Real/Events/ApplicationEvent.h"
#include "Real/Events/MouseEvent.h"

namespace Real {
	class OrthographicCameraController {
	public:
		OrthographicCameraController(float camAspectRatio, bool camRotation = false);

		void onUpdate(Timestep ts);
		void onEvent(Event& e);

		void resize(const glm::vec2& windowSize);

		OrthographicCamera&       getCamera() { return camera; }
		const OrthographicCamera& getCamera() const { return camera; }

		void  setZoomLevel(float level) { zoomLevel = level; }
		float getZoomLevel() const { return zoomLevel; }

	private:
		bool onMouseScrolled(MouseScrolledEvent& e);
		bool onWindowResized(WindowResizeEvent& e);

		float              aspectRatio;
		float              zoomLevel = 1.0f;
		OrthographicCamera camera;

		bool rotation;

		float     cameraRotation = 0.0f;
		glm::vec3 cameraPosition = {0.0f, 0.0f, 0.0f};

		float cameraTranslationSpeed = 1.0f;
		float cameraRotationSpeed    = 2.0f;
		float cameraZoomMultiplier   = 0.25f;
	};

}
