#include "repch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace RealEngine{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: projectionMatrix(glm::ortho(left,right,bottom, top, -1.0f,1.0f)),
		viewMatrix(1.0f), position(0.0f) {
		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}

	void OrthographicCamera::recalculateViewMatrix() {
		const glm::mat4 transfrom = glm::translate(glm::mat4(1.0f), position) * 
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0,0,1));

		viewMatrix = glm::inverse(transfrom);
		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}
}
