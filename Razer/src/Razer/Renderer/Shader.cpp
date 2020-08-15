#include "RZPCH.h"
#include "Shader.h"

#include "Razer/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace rz {
	Shader* Shader::Create(const std::string& vertSrc, const std::string& fragSrc) {
        switch (Renderer::GetAPI()) {
            case RenderAPI::API::None:
            {
                RZ_CORE_ASSERT(false, "NO Render API selected");
                break;
            }

            case RenderAPI::API::OpenGL:
            {
                return new OpenGLShader(vertSrc, fragSrc);
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