#pragma once

#include "Razer/Core.h"
#include "Razer/Renderer/Buffer.h"

namespace rz {
	class RZAPI VertexArray {
	protected:
		using VertexBufferList = std::vector<Ref<VertexBuffer>>;
		using IndexBufferList = std::vector<Ref<IndexBuffer>>;
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const VertexBufferList& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;
		
		static VertexArray* Create();
	};
}