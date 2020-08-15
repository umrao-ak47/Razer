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
	private:
		unsigned int m_RendererID;
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