#pragma once

#include "Real/Renderer/Texture.h"

#include "glad/glad.h"

namespace Real{
	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(unsigned width, unsigned height);

		inline unsigned int getWidth() const override { return width; }
		unsigned int getHeight() const override { return height; }

		virtual void bind(unsigned int slot = 0) const override;

		void setData(void* data, unsigned size) override;

		bool operator==(const Texture& rhs) const noexcept override;

		~OpenGLTexture2D() override;

	private:
		/*
		 * TODO : if there will be ay asset manager, move this path inside it.
		 * for now it's a temp. solution that anyways will be only in debug build
		 * And it will be only here for hot texture reload functionality. Hope so.
		 */
		std::string filePath;

		unsigned int width, height;
		unsigned int rendererId;

		GLenum openGLStoreFormat, dataUsageFormat;
	};
}

