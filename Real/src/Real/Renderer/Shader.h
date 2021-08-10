#pragma once
#include <string>

namespace Real {
	class Shader{
	public:
		virtual ~Shader() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual const std::string& getName() const = 0;

		static Ref<Shader> create(const std::string& filePath);
		static Ref<Shader> create(const std::string& name, const std::string& vertexSrc, const std::string& pixelSrc);
	};

	class ShaderLibrary {
	public:

		void add(const Ref<Shader>& shader);
		void add(const std::string& name, const Ref<Shader>& shader);
		
		Ref<Shader> load(const std::string& filePath);
		Ref<Shader> load(const std::string name, const std::string& filePath);

		Ref<Shader> get(const std::string& name);

		bool isExists(const std::string& name);

	private:
		std::unordered_map<std::string, Ref<Shader>> shaders;
	};
}
