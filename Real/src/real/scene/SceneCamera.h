#pragma once

#include "real/renderer/Camera.h"

namespace Real {
	class SceneCamera final : public Camera {
	public:
		enum class ProjectionType : unsigned { Perspective	= 0,
											   Orthographic = 1 };

		SceneCamera();

		void setOrthographic(float size, float nearClip, float farClip);
		void setPerspective(float verticalFOV, float nearClip, float farClip);
		void setViewPortSize(glm::vec2 size);

		ProjectionType projectionType() const { return type; }
		void		   setProjectionType(ProjectionType t) { type = t; }

		float orthographicSize() const;
		float orthographicNear() const;
		float orthographicFar() const;

		void setOrthographicSize(float size);
		void setOrthographicNear(float nearClip);
		void setOrthographicFar(float farClip);

		float perspectiveVerticalFOV() const;
		float perspectiveNear() const;
		float perspectiveFar() const;

		void setPerspectiveVerticalFOV(float vericalFov);
		void setPerspectiveNear(float nearClip);
		void setPerspectiveFar(float farClip);

	private:
		void recalculateProjection();

		ProjectionType type = ProjectionType::Orthographic;

		float orthoSize = 10.0f;
		float orthoNear = -1.0f, orthoFar = 1.0f;

		float perspecFOV  = glm::radians(60.0f);
		float perspecNear = 0.01f, perspecFar = 1'000.0f;

		float aspectRatio = 0.0f;
	};
}
