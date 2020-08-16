#include "RZPCH.h"
#include "OpenGLArray.h"

#include <glad/glad.h>

namespace rz {
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type) {
			case ShaderDataType::None:        return GL_NONE;
			case ShaderDataType::BOOL:        return GL_BOOL;
			case ShaderDataType::CHAR:        return GL_BYTE;
			case ShaderDataType::UINT:        return GL_UNSIGNED_INT;
			case ShaderDataType::UINT2:       return GL_UNSIGNED_INT;
			case ShaderDataType::UINT3:       return GL_UNSIGNED_INT;
			case ShaderDataType::UINT4:       return GL_UNSIGNED_INT;
			case ShaderDataType::INT:         return GL_INT;
			case ShaderDataType::INT2:        return GL_INT;
			case ShaderDataType::INT3:        return GL_INT;
			case ShaderDataType::INT4:        return GL_INT;
			case ShaderDataType::FLOAT:       return GL_FLOAT;
			case ShaderDataType::FLOAT2:      return GL_FLOAT;
			case ShaderDataType::FLOAT3:      return GL_FLOAT;
			case ShaderDataType::FLOAT4:      return GL_FLOAT;
			case ShaderDataType::MAT2:        return GL_FLOAT;
			case ShaderDataType::MAT3:        return GL_FLOAT;
			case ShaderDataType::MAT4:        return GL_FLOAT;
		}
		RZ_CORE_ASSERT(false, "Unknown Shader Data Type");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray() {
		glCreateVertexArrays(1, &m_RendererID);
		// glBindVertexArray(m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const {
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const {
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(VertexBufferPtr& vertexBuffer) {
		glBindVertexArray(m_RendererID);

		vertexBuffer->Bind();
		const auto& layout = vertexBuffer->GetLayout();
		RZ_CORE_ASSERT(layout->GetElements().size(), "Vertex Buffer has no layout")
		unsigned int index = 0;
		for (const auto& element : layout->GetElements()) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.Count, ShaderDataTypeToOpenGLBaseType(element.Type),
				(element.Normalized ? GL_TRUE : GL_FALSE), layout->GetStride(), (const void*)element.Offset);
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::SetIndexBuffer(IndexBufferPtr& indexBuffer) {
		glBindVertexArray(m_RendererID);

		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
		glBindVertexArray(0);
	}
}

