#include "RZPCH.h"
#include "Texture.h"

#include "Razer/Renderer/RenderAPI.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace rz {
	Texture* Texture::Create(const std::string& file)
	{
        switch (RenderAPI::GetAPI()) {
            case RenderAPI::API::None:
            {
                RZ_CORE_ASSERT(false, "NO Render API selected");
                break;
            }

            case RenderAPI::API::OpenGL:
            {
                return new OpenGLTexture(file);
                break;
            }

            default:
            {
                RZ_CORE_ASSERT(false, "Unknown Render API selected");
            }
        }
		return nullptr;
	}
}

