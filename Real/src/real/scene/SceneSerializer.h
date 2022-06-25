#pragma once

#include "real/core/Core.h"
#include "real/scene/Scene.h"

namespace Real {
	class SceneSerializer final{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void serialize(std::string_view filepath);
		void serializeRuntime(std::string_view filepath);

		bool deserialize(std::string_view filepath);
		bool deserializeRuntime(std::string_view filepath);

	private:
		Ref<Scene> scene;
	};
}
