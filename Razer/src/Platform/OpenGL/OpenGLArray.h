#pragma once

#include "Razer/Renderer/Array.h"
#include <vector>
#include <memory>

namespace rz {
	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		void Bind() const override;
		void Unbind() const override;

		void AddVertexBuffer(VertexBufferPtr& vertexBuffer) override;
		void SetIndexBuffer(IndexBufferPtr& indexBuffer) override;

		const VertexBufferList& GetVertexBuffers() const override { return m_VertexBuffers; }
		const IndexBufferPtr& GetIndexBuffer() const override { return m_IndexBuffer; }
	private:
		unsigned int m_RendererID;
		VertexBufferList m_VertexBuffers;
		IndexBufferPtr m_IndexBuffer;
	};
}