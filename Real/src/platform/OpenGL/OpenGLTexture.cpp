#include "repch.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Real {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: filePath(path) {
		int width_, height_, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data =  stbi_load(path.c_str(),&width_, &height_, &channels, 0);
		RE_CORE_ASSERT(data, "Failed to load texture image");

		width	= static_cast<unsigned int>(width_);
		height	= static_cast<unsigned int>(height_);

		GLenum openGLStoreFormat = 0, dataUsageFormat = 0;
		switch (channels) {
			case 3: 
				openGLStoreFormat = GL_RGB8;
				dataUsageFormat = GL_RGB;
			break;
			case 4: 
				openGLStoreFormat = GL_RGBA8;
				dataUsageFormat = GL_RGBA;
			break;

			default:
				RE_CORE_ASSERT(openGLStoreFormat & dataUsageFormat,
					"Format is not supported");
				break;
			
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &rendererId);
		glTextureStorage2D(rendererId, 1, openGLStoreFormat, width, height);

		glTextureParameteri(rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(rendererId, 0, 0, 0, width, height, dataUsageFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}
	void OpenGLTexture2D::bind(unsigned int slot) const {
		glBindTextureUnit(slot, rendererId);
	}
	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &rendererId);
	}
}
