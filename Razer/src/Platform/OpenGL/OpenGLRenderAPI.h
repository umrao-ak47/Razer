#pragma once

#include "Razer/Renderer/RenderAPI.h"

namespace rz {
	class OpenGLRenderAPI : public RenderAPI {
	public:
		void ClearColor(const glm::vec4& color) override;
		void Clear() override;

		void DrawElements(std::shared_ptr<VertexArray>& vertexArray) override;
	};
}