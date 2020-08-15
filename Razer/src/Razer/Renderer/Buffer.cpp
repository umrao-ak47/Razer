#include "RZPCH.h"
#include "Buffer.h"

#include "Razer/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"


namespace rz {
    //|++++++++++++++++++++++++++++++++++++++++++++++++++|
    //|----------------- Vertex Buffer ------------------|
    //|++++++++++++++++++++++++++++++++++++++++++++++++++|


    VertexBuffer* rz::VertexBuffer::Create(const float* vertices, unsigned int size)
    {
        switch (Renderer::GetAPI()) {
            case RenderAPI::None: 
            {
                RZ_CORE_ASSERT(false, "NO Render API selected");
                break;
            }
            
            case RenderAPI::OpenGL:
            {
                return new OpenGLVertexBuffer(vertices, size);
                break;
            }

            default:
            {
                RZ_CORE_ASSERT(false, "Unknown Render API selected");
            }
        }
        return nullptr;
    }


    //|++++++++++++++++++++++++++++++++++++++++++++++++++|
    //|----------------- Index Buffer -------------------|
    //|++++++++++++++++++++++++++++++++++++++++++++++++++|

    IndexBuffer* rz::IndexBuffer::Create(const unsigned int* indices, unsigned int count)
    {
        switch (Renderer::GetAPI()) {
            case RenderAPI::None:
            {
                RZ_CORE_ASSERT(false, "NO Render API selected");
                break;
            }

            case RenderAPI::OpenGL:
            {
                return new OpenGLIndexBuffer(indices, count);
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


