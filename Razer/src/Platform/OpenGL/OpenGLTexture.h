#pragma once

#include "Razer/Renderer/Texture.h"

namespace rz {
	class OpenGLTexture : public Texture {
	public:
		OpenGLTexture(const std::string& file);
		~OpenGLTexture();

		void Bind(unsigned int slot = 0) const override;
		void Unbind() const override;
	private:
		void Init(const std::string& file);
	private:
		unsigned int m_RendererID;
		Image m_Image;
	};
}