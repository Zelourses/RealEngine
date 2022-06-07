#pragma once

#include "Scene.h"
#include <entt/entt.hpp>

namespace Real {
	class Entity {
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);

		template <typename ComponentType, typename... Args>
		ComponentType& add(Args&&... args) {
			RE_CORE_ASSERT(!allOf<ComponentType>(), "Entity already has this component");
			return scene->registry.emplace<ComponentType>(handle, std::forward<Args>(args)...);
		}

		template <typename ComponentType>
		ComponentType& get() {
			RE_CORE_ASSERT(allOf<ComponentType>(), "Entity does not contains requested component");
			return scene->registry.get<ComponentType>(handle);
		}

		template <typename... ComponentType>
		bool allOf() {
			return scene->registry.all_of<ComponentType...>(handle);
		}

		template <typename ComponentType>
		bool remove() {
			RE_CORE_ASSERT(allOf<ComponentType>(), "Entity does not contains requested component");
			return scene->registry.remove<ComponentType>(handle);
		}

	private:
		entt::entity handle = entt::null;

		// FIXME: rewrite that to use some kind of weak_ptr
		Scene* scene = nullptr;
	};
}
