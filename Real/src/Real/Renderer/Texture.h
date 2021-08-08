#pragma once

namespace Real {
	class Texture {
	public:
		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;

		virtual void bind(unsigned int slot = 0) const = 0;

		
		virtual ~Texture() = default;
	};

	class Texture2D : public Texture{
	public:
		static Ref<Texture2D> create(const std::string& path);
	};
}
