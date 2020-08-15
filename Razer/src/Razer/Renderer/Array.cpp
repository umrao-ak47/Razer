#include "RZPCH.h"
#include "Array.h"

#include "Razer/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLArray.h"

namespace rz {
	VertexArray* rz::VertexArray::Create()
	{
        switch (Renderer::GetAPI()) {
            case RenderAPI::None:
            {
                RZ_CORE_ASSERT(false, "NO Render API selected");
                break;
            }

            case RenderAPI::OpenGL:
            {
                return new OpenGLVertexArray();
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

