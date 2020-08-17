#include "RZPCH.h"
#include "RenderAPI.h"

#include "Platform/OpenGL/OpenGLRenderAPI.h"
#include "Razer/Core/Log.h"

namespace rz {
	RenderAPI::API RenderAPI::s_API = RenderAPI::API::OpenGL;

	RenderAPI* RenderAPI::Get() {
        switch (RenderAPI::GetAPI()) {
            case RenderAPI::API::None:
            {
                RZ_CORE_ASSERT(false, "NO Render API selected");
                break;
            }

            case RenderAPI::API::OpenGL:
            {
                return new OpenGLRenderAPI();
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