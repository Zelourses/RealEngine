#pragma once

#include "Entity.h"

namespace Real {
	class ScriptableEntity {
	public:

		template <typename ComponentType>
		ComponentType& get() {
			return entity.get<ComponentType>();
		}

	private:
		Entity entity;
		friend class Scene;
	};
}
