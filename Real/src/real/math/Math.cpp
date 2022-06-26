#include "repch.h"
#include "Math.h"

#define GL_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace Real::Math {
	bool decomposeTransfrom(const glm::mat4& transform,
							glm::vec3&		 out_translation,
							glm::vec3&		 out_rotation,
							glm::vec3&		 out_scale) {
		//Code got from glm::decompose file
		using namespace glm;
		using T = float;

		mat4 localMatrix(transform);

		//Normalize the matrix
		if (epsilonEqual(localMatrix[3][3], static_cast<T>(0), epsilon<T>())) {
			return false;
		}

		//First, isolate perspective. This is the messiest.
		if (
			epsilonNotEqual(localMatrix[0][3], static_cast<T>(0), epsilon<T>())
			|| epsilonNotEqual(localMatrix[1][3], static_cast<T>(0), epsilon<T>())
			|| epsilonNotEqual(localMatrix[2][3], static_cast<T>(0), epsilon<T>())) {
			//clear the perspective partition
			localMatrix[0][3] = localMatrix[1][3] = localMatrix[2][3] = static_cast<T>(0);
			localMatrix[3][3]										  = static_cast<T>(1);
		}

		//Next take care of translation (easy).
		out_translation = vec3(localMatrix[3]);
		localMatrix[3]	= vec4(0, 0, 0, localMatrix[3].w);

		vec3 row[3];

		//now get scale and shear.
		for (length_t i = 0; i < 3; ++i) {
			for (length_t j = 0; j < 3; ++j) {
				row[i][j] = localMatrix[i][j];
			}
		}

		//Compoute x scale factor and normalize first row.
		out_scale.x = length(row[0]);
		row[0]		= detail::scale(row[0], static_cast<T>(1));
		out_scale.y = length(row[1]);
		row[1]		= detail::scale(row[1], static_cast<T>(1));
		out_scale.z = length(row[2]);
		row[2]		= detail::scale(row[2], static_cast<T>(1));

		//here is missing coordinate space flip check

		out_rotation.y = asin(-row[0][2]);
		if (cos(out_rotation.y) != 0) {
			out_rotation.x = atan2(row[1][2], row[2][2]);
			out_rotation.z = atan2(row[0][1], row[0][0]);
		} else {
			out_rotation.x = atan2(-row[2][0], row[1][1]);
			out_rotation.z = 0;
		}
		return true;
	}
}