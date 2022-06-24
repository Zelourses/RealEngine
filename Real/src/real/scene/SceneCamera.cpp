#include "repch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Real {
	SceneCamera::SceneCamera()
		: Camera(glm::mat4(1.0f)) {
		recalculateProjection();
	}
	void SceneCamera::setOrthographic(float size, float nearClip, float farClip) {
		type	  = ProjectionType::Orthographic;
		orthoSize = size;
		orthoNear = nearClip;
		orthoFar  = farClip;

		recalculateProjection();
	}
	void SceneCamera::setPerspective(float verticalFOV, float nearClip, float farClip) {
		type		= ProjectionType::Perspective;
		perspecFOV	= verticalFOV;
		perspecNear = nearClip;
		perspecFar	= farClip;

		recalculateProjection();
	}
	void SceneCamera::setViewPortSize(glm::vec2 size) {
		aspectRatio = size.x
					/ //-------
					  size.y;
		recalculateProjection();
	}
	void SceneCamera::recalculateProjection() {
		switch (type) {
			case ProjectionType::Orthographic: {
				float right	 = orthoSize * aspectRatio * 0.5f;
				float left	 = -orthoSize * aspectRatio * 0.5f;
				float top	 = orthoSize * 0.5f;
				float bottom = -orthoSize * 0.5f;

				projectionMatrix = glm::ortho(left, right, bottom, top, orthoNear, orthoFar);
				break;
			}
			case ProjectionType::Perspective: {
				projectionMatrix = glm::perspective(perspecFOV,aspectRatio, perspecNear, perspecFar);
				break;
			}
			default:
				RE_CORE_ASSERT(false, "Unknown type of projectionType");
				break;
		}
	}

	float SceneCamera::orthographicSize() const {
		return orthoSize;
	}

	float SceneCamera::orthographicNear() const {
		return orthoNear;
	}

	float SceneCamera::orthographicFar() const {
		return orthoFar;
	}

	void SceneCamera::setOrthographicSize(float size) {
		orthoSize = size;
		recalculateProjection();
	}
	void SceneCamera::setOrthographicNear(float nearClip) {
		orthoNear = nearClip;
		recalculateProjection();
	}

	void SceneCamera::setOrthographicFar(float farClip) {
		orthoFar = farClip;
		recalculateProjection();
	}
	float SceneCamera::perspectiveVerticalFOV() const {
		return perspecFOV;
	}
	float SceneCamera::perspectiveNear() const {
		return perspecNear;
	}
	float SceneCamera::perspectiveFar() const {
		return perspecFar;
	}
	void SceneCamera::setPerspectiveVerticalFOV(float vericalFov) {
		perspecFOV = vericalFov;
		recalculateProjection();
	}
	void SceneCamera::setPerspectiveNear(float nearClip) {
		perspecNear = nearClip;
		recalculateProjection();
	}
	void SceneCamera::setPerspectiveFar(float farClip) {
		perspecFar = farClip;
		recalculateProjection();
	}
}