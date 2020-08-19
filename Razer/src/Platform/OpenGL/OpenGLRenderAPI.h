#pragma once

#include "Razer/Renderer/RenderAPI.h"

namespace rz {
	class OpenGLRenderAPI : public RenderAPI {
	public:
		void ClearColor(const glm::vec4& color) override;
		void Clear() override;

		void SetViewPort(int width, int height) override;
		void DrawElements(const Ref<VertexArray>& vertexArray) override;
	};
}