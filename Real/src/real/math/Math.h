#pragma once

#include <glm/glm.hpp>

namespace Real::Math {
	/**
	 * @brief more lightweit version of glm::decompose that works only with transform
	 * @param transform 
	 * @param out_translation 
	 * @param out_rotation 
	 * @param scale 
	 * @return if result is written in out parameters
	 * 
	 * DISCUSS: maybe change signature of function to optional<tuple<vec3,vec3,vec3>>?
	*/
	bool decomposeTransfrom(const glm::mat4& transform,
							glm::vec3&		 out_translation,
							glm::vec3&		 out_rotation,
							glm::vec3&		 out_scale);
}
