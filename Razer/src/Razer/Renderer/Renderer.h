#pragma once

#include "Razer/Core.h"
#include "Razer/Renderer/Array.h"
#include "Razer/Renderer/RenderAPI.h"
#include "Razer/Renderer/RendererCommand.h"
#include "Razer/Renderer/Shader.h"
#include "Razer/Renderer/Texture.h"
#include "Razer/Renderer/Camera.h"
#include <glm/glm.hpp>

namespace rz {
	class RZAPI Renderer {
	public:
		static void BeginScene(const Camera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray);

		static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); }
	private:
		struct SceneData {
			glm::mat4 ProjectionViewMatrix;
		};

		static SceneData* m_Data;
	};
}