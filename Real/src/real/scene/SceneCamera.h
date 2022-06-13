#pragma once

#include "real/renderer/Camera.h"

namespace Real {
	class SceneCamera final : public Camera {
	public:
		SceneCamera();

		void setOrthographic(float size, float nearClip, float farClip);
		void setViewPortSize(glm::vec2 size);

	private:
		void recalculateProjection();

		float orthographicSize = 10.0f;
		float orthographicNear = -1.0f, orthographicFar = 1.0f;
		float aspectRatio = 0.0f;
	};
}
