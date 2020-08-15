#pragma once

#include "Razer/Renderer/Shader.h"

namespace rz {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& vertSrc, const std::string& fragSrc);
		~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;
	private:
		void Init(const std::string& vertSrc, const std::string& fragSrc);
	private:
		unsigned int m_RendererID;
	};
}