#pragma once

#include "Entity.h"

namespace Real {
	class ScriptableEntity {
	public:

		template <typename ComponentType>
		ComponentType& get() {
			return entity.get<ComponentType>();
		}

		virtual ~ScriptableEntity() = default;

	protected:
		virtual void onCreate() {}
		virtual void onUpdate(Timestep ts) {}
		virtual void onDestroy() {}


	private:
		Entity entity;
		friend class Scene;
	};
}
