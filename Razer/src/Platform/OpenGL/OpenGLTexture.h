#pragma once

#include "Razer/Renderer/Texture.h"

namespace rz {
	class OpenGLTexture : public Texture {
	public:
		OpenGLTexture(const std::string& file, bool transparent);
		~OpenGLTexture();

		void Bind() const override;
		void Unbind() const override;
	private:
		void Init(const std::string& file, bool transparent);
	private:
		unsigned int m_RendererID;
		Image m_Image;
	};
}