#include "RZPCH.h"
#include "Shader.h"

#include "Razer/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Razer/Core/Log.h"

namespace rz {
    Ref<Shader> Shader::Create(const std::string& filepath) {
        switch (Renderer::GetAPI()) {
        case RenderAPI::API::None:
        {
            RZ_CORE_ASSERT(false, "NO Render API selected");
            break;
        }

        case RenderAPI::API::OpenGL:
        {
            return Ref<Shader>(new OpenGLShader(filepath));
            break;
        }

        default:
        {
            RZ_CORE_ASSERT(false, "Unknown Render API selected");
        }
        }
        return nullptr;
    }

    Ref<Shader> Shader::Create(const std::string& name, const std::string& filepath) {
        switch (Renderer::GetAPI()) {
            case RenderAPI::API::None:
            {
                RZ_CORE_ASSERT(false, "NO Render API selected");
                break;
            }

            case RenderAPI::API::OpenGL:
            {
                return Ref<Shader>(new OpenGLShader(name, filepath));
                break;
            }

            default:
            {
                RZ_CORE_ASSERT(false, "Unknown Render API selected");
            }
        }
        return nullptr;
    }

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertSrc, const std::string& fragSrc) {
        switch (Renderer::GetAPI()) {
            case RenderAPI::API::None:
            {
                RZ_CORE_ASSERT(false, "NO Render API selected");
                break;
            }

            case RenderAPI::API::OpenGL:
            {
                return Ref<Shader>(new OpenGLShader(name, vertSrc, fragSrc));
                break;
            }

            default:
            {
                RZ_CORE_ASSERT(false, "Unknown Render API selected");
            }
        }
        return nullptr;
	}

    // Shader Manager Stuff
    Ref<ShaderManager> ShaderManager::s_Instance = nullptr;

    void ShaderManager::Add(const Ref<Shader>& shader) {
        std::string name = shader->GetName();
        RZ_CORE_ASSERT(!Exists(name), "Shader Already Exists");
        m_Shaders[name] = shader;
    }

    Ref<Shader> ShaderManager::Load(const std::string& filepath) {
        Ref<Shader> shader = Shader::Create(filepath);
        Add(shader);
        return shader;
    }

    Ref<Shader> ShaderManager::Load(const std::string& name, const std::string& filepath) {
        Ref<Shader> shader = Shader::Create(name, filepath);
        Add(shader);
        return shader;
    }

    Ref<Shader> ShaderManager::Load(const std::string& name, const std::string& vertSrc, const std::string& fragSrc) {
        Ref<Shader> shader = Shader::Create(name, vertSrc, fragSrc);
        Add(shader);
        return shader;
    }

    Ref<Shader> ShaderManager::Get(const std::string& name) const {
        if (!Exists(name)) {
            RZ_CORE_ERROR("No Shader exists with this name {0}", name);
            return nullptr;
        }
        return m_Shaders.at(name);
    }
}