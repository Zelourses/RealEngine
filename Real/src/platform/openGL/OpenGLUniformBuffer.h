#pragma once
#include "real/renderer/UniformBuffer.h"

namespace Real {
	class OpenGLUniformBuffer : public UniformBuffer {
	public:
		OpenGLUniformBuffer(size_t size, unsigned binding);

		virtual void setData(const void* data, size_t size, size_t offset) override;

		~OpenGLUniformBuffer();

	private:
		unsigned rendererId;
	};
}