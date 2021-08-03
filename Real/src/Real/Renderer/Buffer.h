#pragma once

namespace Real {

	enum class ShaderDataType {
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool,
			
	};

	using SDT = ShaderDataType;

	static uint32_t shaderDataTypeSize(ShaderDataType type) {
		switch (type) {
			case SDT::None: /*it's also an error*/break;
			case SDT::Float:	return 4;
			case SDT::Float2:	return 4 * 2;
			case SDT::Float3:	return 4 * 3;
			case SDT::Float4:	return 4 * 4;
			case SDT::Mat3:		return 4 * 3 * 3;
			case SDT::Mat4:		return 4 * 4 * 4;
			case SDT::Int:		return 4;
			case SDT::Int2:		return 4 * 2;
			case SDT::Int3:		return 4 * 3;
			case SDT::Int4:		return 4 * 4;
			case SDT::Bool:		return 1; //May be changed?
		}
		
		RE_CORE_ASSERT(false, "Unknown data type");
		return 0;
	}

	struct BufferElement {
		std::string name;
		ShaderDataType	type;
		uint32_t	size;
		uint32_t	offset;
		bool normalized;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false )
			:name(name), type(type),size(shaderDataTypeSize(type)), offset(0), normalized(normalized) {
		}

		uint32_t getComponentCount() const {
			switch (type) {
				case SDT::None:		break;
				case SDT::Float:	return 1;
				case SDT::Float2:	return 2;
				case SDT::Float3:	return 3;
				case SDT::Float4:	return 4;
				case SDT::Mat3:		return 3 * 3;
				case SDT::Mat4:		return 4 * 4;
				case SDT::Int:		return 1;
				case SDT::Int2:		return 2;
				case SDT::Int3:		return 3;
				case SDT::Int4:		return 4;
				case SDT::Bool:		return 1;
			}

			RE_CORE_ASSERT(false, "unknown ShaderDataType");
			return 0;
		}
	};

	class BufferLayout {
	public:

		BufferLayout() = default;

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			:elements(elements){
			calculateOffsetAndStride();
		}
		
		inline const std::vector<BufferElement>& getElements() const { return elements; }
		inline uint32_t getStride() const { return stride; }

		std::vector<BufferElement>::iterator begin() { return elements.begin(); }
		std::vector<BufferElement>::iterator end() { return elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return elements.cbegin(); }
		std::vector<BufferElement>::const_iterator end() const { return elements.cend(); }
	private:

		void calculateOffsetAndStride() {
			uint32_t offset = 0;
			stride = 0;
			for (auto& el: elements) {
				el.offset = offset;
				offset += el.size;
				stride += el.size;
			}
		}
		std::vector<BufferElement> elements;
		uint32_t stride = 0;
	};

	class VertexBuffer {
	public:

		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual void setLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& getLayout() const = 0;

		static VertexBuffer* create(float* vertices, uint32_t size);
		
		virtual ~VertexBuffer() {}
	};

	class IndexBuffer {
	public:
		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		//how many indices in buffer
		virtual uint32_t getCount() const = 0;


		static IndexBuffer* create(uint32_t* indices, uint32_t size);

		virtual ~IndexBuffer() {}
		
	};
}
