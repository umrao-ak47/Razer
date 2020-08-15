#pragma once

#include "Razer/Core.h"
#include "Razer/Renderer/Buffer.h"

namespace rz {
	class RZAPI VertexArray {
	protected:
		using VertexBufferList = std::vector<std::shared_ptr<VertexBuffer>>;
		using IndexBufferList = std::vector<std::shared_ptr<IndexBuffer>>;
		using VertexBufferPtr = std::shared_ptr<VertexBuffer>;
		using IndexBufferPtr = std::shared_ptr<IndexBuffer>;
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(VertexBufferPtr& vertexBuffer) = 0;
		virtual void SetIndexBuffer(IndexBufferPtr& indexBuffer) = 0;

		virtual const VertexBufferList& GetVertexBuffers() const = 0;
		virtual const IndexBufferPtr& GetIndexBuffer() const = 0;
		
		static VertexArray* Create();
	};
}