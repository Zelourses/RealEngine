#include <repch.h>
#include "SceneSerializer.h"

#include "Entity.h"
#include "Components.h"

#include <yaml-cpp/yaml.h>

namespace YAML {
	template <>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}
		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3) {
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();

			return true;
		}
	};

	template <>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}
		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4) {
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();

			return true;
		}
	};

}

namespace /*anonymous*/ {
	using namespace Real; // it's internal code for engine, it's allowed here

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
		out << YAML::Flow
			<< YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
		out << YAML::Flow
			<< YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	void serializeEntity(YAML::Emitter& out, Entity entity) {
		out << YAML::BeginMap
			<< YAML::Key << "Entity" << YAML::Value << 0xDEADBEEF; //TODO:Entity id

		//There is always a tag
		out << YAML::Key << "TagComponent" << YAML::Value
			<< YAML::BeginMap
			<< YAML::Key << "Tag" << YAML::Value << entity.get<TagComponent>().tag
			<< YAML::EndMap;

		//I am not sure about transform component, so it will be like this
		if (entity.allOf<TransformComponent>()) {
			auto&& tc = entity.get<TransformComponent>();

			out << YAML::Key << "TransformComponent" << YAML::Value
				<< YAML::BeginMap
				<< YAML::Key << "Translation" << YAML::Value << tc.translation
				<< YAML::Key << "Rotation" << YAML::Value << tc.rotation
				<< YAML::Key << "Scale" << YAML::Value << tc.scale
				<< YAML::EndMap;
		}

		if (entity.allOf<CameraComponent>()) {
			auto&& cameraComponent = entity.get<CameraComponent>();
			auto&& cam			   = cameraComponent.camera;

			out << YAML::Key << "CameraComponent" << YAML::Value
				<< YAML::BeginMap

				<< YAML::Key << "Camera" << YAML::Value
				<< YAML::BeginMap
				<< YAML::Key << "ProjectionType" << YAML::Value << static_cast<unsigned>(cam.projectionType())

				<< YAML::Key << "PerspectiveVerticalFOV" << YAML::Value << cam.perspectiveVerticalFOV()
				<< YAML::Key << "PerspectiveNear" << YAML::Value << cam.perspectiveNear()
				<< YAML::Key << "PerspectiveFar" << YAML::Value << cam.perspectiveFar()

				<< YAML::Key << "OrthographicSize" << YAML::Value << cam.orthographicSize()
				<< YAML::Key << "OrthographicNear" << YAML::Value << cam.orthographicNear()
				<< YAML::Key << "OrthographicFar" << YAML::Value << cam.orthographicFar()
				<< YAML::EndMap

				<< YAML::Key << "Primary" << YAML::Value << cameraComponent.primary
				<< YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.fixedAspectRatio

				<< YAML::EndMap;
		}

		if (entity.allOf<SpriteRendererComponent>()) {
			auto&& spriteRendererComponent = entity.get<SpriteRendererComponent>();

			out << YAML::Value << "SpriteRendererComponent" << YAML::Value
				<< YAML::BeginMap
				<< YAML::Key << "Color" << YAML::Value << spriteRendererComponent.color
				<< YAML::EndMap;
		}

		//End of entity
		out << YAML::EndMap;
	}
}

namespace Real {
	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: scene(scene) {}

	void SceneSerializer::serialize(std::string_view filepath) {
		YAML::Emitter out;

		out << YAML::BeginMap
			<< YAML::Key << "Scene" << YAML::Value << "Unnamed scene"
			<< YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		scene->registry.each([&out, this](auto&& entityID) {
			auto entity = Entity{entityID, scene.get()};
			if (!entity)
				return;
			serializeEntity(out, entity);
		});

		out << YAML::EndSeq
			<< YAML::EndMap;


		std::ofstream fout(filepath.data());
		fout << out.c_str();
	}
	void SceneSerializer::serializeRuntime(std::string_view filepath) {
		//unimplemented
		RE_CORE_ASSERT(false);
	}
	bool SceneSerializer::deserialize(std::string_view filepath) {
		auto&& data = YAML::LoadFile(filepath.data());
		if (!data["Scene"]) {
			RE_CORE_ERROR("'Scene' doesn't found in file {0}, while trying to deserialize scene", filepath);
			return false;
		}

		auto sceneName = data["Scene"].as<std::string>();
		RE_CORE_INFO("Deserializing scene '{0}'", sceneName);

		auto&& listOfEntities = data["Entities"];

		for (auto&& entity: listOfEntities) {
			auto&& uuid = entity["Entity"].as<uint64_t>(); // TODO: use it (and, obviously, write that)

			//And again, tag component must be always
			auto name = entity["TagComponent"]["Tag"].as<std::string>();

			auto deserializedEntity = scene->createEntity(name);

			//actually transform component must be always
			auto transformComponent = entity["TransformComponent"];
			if (transformComponent) {
				auto&& tc = deserializedEntity.get<TransformComponent>();

				tc.translation = transformComponent["Translation"].as<glm::vec3>();
				tc.rotation	   = transformComponent["Rotation"].as<glm::vec3>();
				tc.scale	   = transformComponent["Scale"].as<glm::vec3>();
			}

			auto cameraComponent = entity["CameraComponent"];
			if (cameraComponent) {
				auto&& cc = deserializedEntity.add<CameraComponent>();

				auto&& cameraProps = cameraComponent["Camera"];

				cc.camera.setProjectionType(static_cast<
					SceneCamera::ProjectionType>(cameraProps["ProjectionType"].as<unsigned>()
				));

				cc.camera.setPerspectiveVerticalFOV(cameraProps["PerspectiveVerticalFOV"].as<float>());
				cc.camera.setPerspectiveNear(cameraProps["PerspectiveNear"].as<float>());
				cc.camera.setPerspectiveFar(cameraProps["PerspectiveFar"].as<float>());

				cc.camera.setOrthographicSize(cameraProps["OrthographicSize"].as<float>());
				cc.camera.setOrthographicNear(cameraProps["OrthographicNear"].as<float>());
				cc.camera.setOrthographicFar(cameraProps["OrthographicFar"].as<float>());

				cc.primary = cameraComponent["Primary"].as<bool>();
				cc.fixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
			}

			auto spriteRendererComponent = entity["SpriteRendererComponent"];
			if (spriteRendererComponent) {
				auto&& src = deserializedEntity.add<SpriteRendererComponent>();
				src.color  = spriteRendererComponent["Color"].as<glm::vec4>();
			}


			RE_CORE_TRACE("Deserialized entity with ID = '{0}', name = '{1}'", uuid, name);
		}
		return true;
	}

	bool SceneSerializer::deserializeRuntime(std::string_view filepath) {
		//unimplemented
		RE_CORE_ASSERT(false);
		return false;
	}
}