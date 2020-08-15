#pragma once

#include "Razer/Core.h"
#include "Razer/Renderer/Array.h"
#include "Razer/Renderer/RenderAPI.h"
#include "Razer/Renderer/RendererCommand.h"

namespace rz {
	class RZAPI Renderer {
	public:
		static void BeginScene();
		static void EndScene();

		static void Submit(std::shared_ptr<VertexArray>& vertexArray);

		static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); }
	};
}