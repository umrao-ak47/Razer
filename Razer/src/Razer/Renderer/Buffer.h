#pragma once

#include "Razer/Core.h"
#include "Razer/Core/Log.h"


namespace rz {
	enum class ShaderDataType {
		None = 0,
		BOOL, CHAR,
		UINT, UINT2, UINT3, UINT4,
		INT, INT2, INT3, INT4,
		FLOAT, FLOAT2, FLOAT3, FLOAT4,
		MAT2, MAT3, MAT4,
		SAMPLER2D
	};

	static unsigned int ShaderDataTypeCount(ShaderDataType type) {
		switch (type) {
			case ShaderDataType::None:        return 0;
			case ShaderDataType::BOOL:        return 1;
			case ShaderDataType::CHAR:        return 1;
			case ShaderDataType::UINT:        return 1;
			case ShaderDataType::UINT2:       return 2;
			case ShaderDataType::UINT3:       return 3;
			case ShaderDataType::UINT4:       return 4;
			case ShaderDataType::INT:         return 1;
			case ShaderDataType::INT2:        return 2;
			case ShaderDataType::INT3:        return 3;
			case ShaderDataType::INT4:        return 4;
			case ShaderDataType::FLOAT:       return 1;
			case ShaderDataType::FLOAT2:      return 2;
			case ShaderDataType::FLOAT3:      return 3;
			case ShaderDataType::FLOAT4:      return 4;
			case ShaderDataType::MAT2:        return 2 * 2;
			case ShaderDataType::MAT3:        return 3 * 3;
			case ShaderDataType::MAT4:        return 4 * 4;
		}

		RZ_CORE_ASSERT(false, "ShaderDataUnknown Shader DataType");
		return 0;
	}

	static unsigned int ShaderDataTypeSize(ShaderDataType type) {
		switch (type) {
			case ShaderDataType::None:        return 0;
			case ShaderDataType::BOOL:        return 1 * sizeof(bool);
			case ShaderDataType::CHAR:        return 1 * sizeof(char);
			case ShaderDataType::UINT:        return 1 * sizeof(unsigned int);
			case ShaderDataType::UINT2:       return 2 * sizeof(unsigned int);
			case ShaderDataType::UINT3:       return 3 * sizeof(unsigned int);
			case ShaderDataType::UINT4:       return 4 * sizeof(unsigned int);
			case ShaderDataType::INT:         return 1 * sizeof(int);
			case ShaderDataType::INT2:        return 2 * sizeof(int);
			case ShaderDataType::INT3:        return 3 * sizeof(int);
			case ShaderDataType::INT4:        return 4 * sizeof(int);
			case ShaderDataType::FLOAT:       return 1 * sizeof(float);
			case ShaderDataType::FLOAT2:      return 2 * sizeof(float);
			case ShaderDataType::FLOAT3:      return 3 * sizeof(float);
			case ShaderDataType::FLOAT4:      return 4 * sizeof(float);
			case ShaderDataType::MAT2:        return 2 * 2 * sizeof(float);
			case ShaderDataType::MAT3:        return 3 * 3 * sizeof(float);
			case ShaderDataType::MAT4:        return 4 * 4 * sizeof(float);
		}

		RZ_CORE_ASSERT(false, "ShaderDataUnknown Shader DataType");
		return 0;
	}

	struct BufferElement {
		std::string Name;
		ShaderDataType Type;
		unsigned int Size;
		unsigned int Offset;
		unsigned int Count;
		bool Normalized;

		BufferElement()
			: Name("None"), Type(ShaderDataType::None), Size(0),
			Offset(0), Count(0), Normalized(false) {}

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)),
			Offset(0), Count(ShaderDataTypeCount(type)), Normalized(normalized) {}
	};

	class RZAPI BufferLayout {
		using BufferList = std::vector<BufferElement>;
	public:
		BufferLayout()
			: m_Stride(0) {}

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements) {
			CalculateOffsetAndStride();
		}

		BufferLayout(const BufferList& elements)
			: m_Elements(elements) {
			CalculateOffsetAndStride();
		}

		unsigned int GetStride() const { return m_Stride; }
		const BufferList& GetElements() const { return m_Elements; }

		BufferList::iterator begin() { return m_Elements.begin(); }
		BufferList::iterator end() { return m_Elements.end(); }
		BufferList::const_iterator begin() const { return m_Elements.begin(); }
		BufferList::const_iterator end() const { return m_Elements.end(); }
	private:
		void CalculateOffsetAndStride() {
			unsigned int offset = 0;
			m_Stride = 0;

			for (auto& element : m_Elements) {
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

	private:
		BufferList m_Elements;
		unsigned int m_Stride;
	};

	//|++++++++++++++++++++++++++++++++++++++++++++++++++|
	//|----------------- Vertex Buffer ------------------|
	//|++++++++++++++++++++++++++++++++++++++++++++++++++|

	class RZAPI VertexBuffer {
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetLayout(const Ref<BufferLayout>& layout) = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		static VertexBuffer* Create(const float* vertices, unsigned int size);
	};


	//|++++++++++++++++++++++++++++++++++++++++++++++++++|
	//|----------------- Index Buffer -------------------|
	//|++++++++++++++++++++++++++++++++++++++++++++++++++|

	class RZAPI IndexBuffer {
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual unsigned int GetCount() const = 0;

		static IndexBuffer* Create(const unsigned int* indices,unsigned int count);
	};
}