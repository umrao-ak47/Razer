#pragma once

#include "Razer/Renderer/Buffer.h"

namespace rz {

	//|++++++++++++++++++++++++++++++++++++++++++++++++++|
	//|-------------- OpenGL Vertex Buffer --------------|
	//|++++++++++++++++++++++++++++++++++++++++++++++++++|

	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(const float* vertices, unsigned int size);
		~OpenGLVertexBuffer();

		void Bind() const override;
		void Unbind() const override;

		void SetLayout(const std::shared_ptr<BufferLayout>& layout) override { m_Layout = layout; }
		const std::shared_ptr<BufferLayout>& GetLayout() const override { return m_Layout; }
	private:
		unsigned int m_RendererID;
		std::shared_ptr<BufferLayout> m_Layout;
	};


	//|++++++++++++++++++++++++++++++++++++++++++++++++++|
	//|-------------- OpenGL Index Buffer ---------------|
	//|++++++++++++++++++++++++++++++++++++++++++++++++++|

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(const unsigned int* indices, unsigned int count);
		~OpenGLIndexBuffer();

		void Bind() const override;
		void Unbind() const override;

		unsigned int GetCount() const override { return m_Count; }
	private:
		unsigned int m_RendererID;
		unsigned int m_Count;
	};
}