#include "repch.h"
#include "Entity.h"

namespace Real {
	Entity::Entity(entt::entity handle, Scene* scene)
	    : handle(handle), scene(scene) {
	}
}