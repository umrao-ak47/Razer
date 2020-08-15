#pragma once

#include "Razer/Core.h"
#include "Razer/Renderer/RenderAPI.h"

namespace rz {
	class RZAPI RendererCommand {
	public:
		static void ClearColor(const glm::vec4& color) {
			s_RenderAPI->ClearColor(color);
		}

		static void Clear() {
			s_RenderAPI->Clear();
		}

		static void DrawElements(std::shared_ptr<VertexArray>& vertexArray) {
			s_RenderAPI->DrawElements(vertexArray);
		}

	private:
		static RenderAPI* s_RenderAPI;
	};
}