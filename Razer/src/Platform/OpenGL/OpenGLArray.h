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

		void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		const VertexBufferList& GetVertexBuffers() const override { return m_VertexBuffers; }
		const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
	private:
		unsigned int m_RendererID;
		VertexBufferList m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}