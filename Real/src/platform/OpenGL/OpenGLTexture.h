#pragma once

#include "Real/Renderer/Texture.h"
namespace Real{
	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const std::string& path);

		inline unsigned int getWidth() const override { return width; }
		unsigned int getHeight() const override { return height; }

		virtual void bind(unsigned int slot = 0) const override;

		
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
	};
}

