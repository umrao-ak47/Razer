#pragma once

#include "Razer/Renderer/Shader.h"

namespace rz {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& vertSrc, const std::string& fragSrc);
		~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;
		
		const std::shared_ptr<BufferLayout>& ExtractLayout() const override { return m_Layout; }
	private:
		void Init(const std::string& vertSrc, const std::string& fragSrc);
		
		void ComputeLayout();
		// temporary
		void PrintInfo();
	private:
		unsigned int m_RendererID;
		std::shared_ptr<BufferLayout> m_Layout;
	};
}