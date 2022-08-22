#pragma once

#include <utility>

#include "Camera.h"
#include "real/core/Timestep.h"
#include "real/events/Event.h"
#include "real/events/MouseEvent.h"

namespace Real {
	class EditorCamera final : public Camera {
	public:
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void onUpdate(Timestep ts);
		void onEvent(Event& e);

		inline float distance() const {
			return viewDistance;
		}
		inline void setDistance(float distance) {
			viewDistance = distance;
		}

		inline void setViewPortSize(const glm::vec2& size) {
			viewportWidth  = size.x;
			viewportHeight = size.y;
			updateProjection();
		}

		inline const glm::mat4& view() const {
			return viewMatrix;
		}
		glm::mat4 viewProjection() const;

		glm::vec3		 upDirection() const;
		glm::vec3		 rightDirection() const;
		glm::vec3		 forwardDirection() const;
		const glm::vec3& position() const;

		glm::quat orientation() const;

		float pitch() const;
		float yaw() const;

	private:
		void updateProjection();
		void updateView();

		bool onMouseScroll(MouseScrolledEvent& e);

		void mousePan(const glm::vec2& delta);
		void mouseRotate(const glm::vec2& delta);
		void mouseZoom(float delta);

		glm::vec3 calculatePosition() const;

		glm::vec2 panSpeed() const;

		float rotationSpeed() const;
		float zoomSpeed() const;

		/**
		 * fov in radians
		*/
		float fov, aspectRatio, nearClip, farClip;

		glm::mat4 viewMatrix;
		glm::vec3 camPosition = {0.0f, 0.0f, 0.0f};
		glm::vec3 focalPoint  = {0.0f, 0.0f, 0.0f};

		glm::vec2 initialMousePos = {0.0f, 0.0f};

		float viewDistance = 10.0f;
		float camPitch = 0.0f, camYaw = 0.0f;

		float viewportWidth = 1280.0f, viewportHeight = 720.0f;
	};
}