#pragma once
namespace Real {
	class UniformBuffer {
	public:

		static Ref<UniformBuffer> create(size_t size, unsigned binding);

		virtual void setData(const void* data, size_t size, size_t offset = 0 ) = 0;

		virtual ~UniformBuffer() = default;
	};
}
