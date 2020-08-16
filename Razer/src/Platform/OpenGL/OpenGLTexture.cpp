#include "RZPCH.h"
#include "OpenGLTexture.h"

#include "Razer/Log.h"
#include <glad/glad.h>
#include <stb_image.h>

namespace rz {
	OpenGLTexture::OpenGLTexture(const std::string& file, bool transparent) {
		glGenTextures(1, &m_RendererID);
		stbi_set_flip_vertically_on_load(true);
		m_Image.data = stbi_load(file.c_str(), &m_Image.Width, &m_Image.Height,
			&m_Image.Channels, 0);
		if (m_Image.data == nullptr) {
			RZ_CORE_ASSERT(false, "Texture Loading Failed");
		}
		RZ_CORE_TRACE("Loaded Image");

		GLenum type = GL_RGB;
		if (m_Image.Channels == 4) {
			type = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Image.Width, m_Image.Height, 0, 
			type, GL_UNSIGNED_BYTE, m_Image.data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Clear image data after use
		//stbi_image_free(m_ImageData.data)
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	OpenGLTexture::~OpenGLTexture() {
		glDeleteTextures(1, &m_RendererID);
		// storing data till lifetime with shader
		stbi_image_free(m_Image.data);
	}

	void OpenGLTexture::Bind() const {
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	void OpenGLTexture::Unbind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLTexture::Init(const std::string& file, bool transparent) {
		
	}
}
