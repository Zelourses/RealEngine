#include "repch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Real {
	SceneCamera::SceneCamera()
		: Camera(glm::mat4(1.0f)) {
		recalculateProjection();
	}
	void SceneCamera::setOrthographic(float size, float nearClip, float farClip) {
		orthographicSize = size;
		orthographicNear = nearClip;
		orthographicFar	 = farClip;

		recalculateProjection();
	}
	void SceneCamera::setViewPortSize(glm::vec2 size) {
		aspectRatio = size.x
					/ //---
					  size.y;
		recalculateProjection();
	}
	void SceneCamera::recalculateProjection() {
		float right	 = orthographicSize * aspectRatio * 0.5f;
		float left	 = -orthographicSize * aspectRatio * 0.5f;
		float top	 = orthographicSize * 0.5f;
		float bottom = -orthographicSize * 0.5f;

		projectionMatrix = glm::ortho(left, right, bottom, top, orthographicNear, orthographicFar);
	}
}