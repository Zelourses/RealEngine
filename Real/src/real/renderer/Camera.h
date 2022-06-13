#pragma once

#include <glm/glm.hpp>

namespace Real {
	struct Camera {
		Camera(const glm::mat4& projectionMatrix)
		    : projectionMatrix(projectionMatrix) {}

		~Camera() = default;

		const glm::mat4& projection() const { return projectionMatrix; }
	protected:
		glm::mat4 projectionMatrix;
	};
}