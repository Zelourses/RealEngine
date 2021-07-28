#pragma once

#include <glm/glm.hpp>

namespace RealEngine{
	class OrthographicCamera {
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void setPosition(const glm::vec3& Position) { position = Position; recalculateViewMatrix(); }
		const glm::vec3& getPosition() const { return position; }

		
		/**
		 * \brief sets the rotation of Orthographic camera
		 * \param Rotation rotation angle in radians
		 */
		void setRotation(const float Rotation) { rotation = Rotation; recalculateViewMatrix();}
		
		/**
		 * \return returns rotation angle in radians
		 */
		float getRotation() const { return rotation; }

		const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }
		const glm::mat4& getViewMatrix() const { return viewMatrix; }
		const glm::mat4& getViewProjectionMatrix() const { return viewProjectionMatrix; }
		
	private:
		void recalculateViewMatrix();
		
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
		glm::mat4 viewProjectionMatrix;

		glm::vec3 position = {0.0f,0.0f,0.0f};
		float rotation = 0.0f; //You can't really rotate the orthographic camera, around X and Y axis
							//So, it will be just a float, that shows rotation on Z axis
	};
}

