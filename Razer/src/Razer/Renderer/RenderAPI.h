#pragma once

#include "Razer/Core.h"
#include "Razer/Renderer/Array.h"
#include <memory>
#include <glm/glm.hpp>

namespace rz {
	class RZAPI RenderAPI {
	public:
		enum RZAPI class API {
			None = 0,
			OpenGL = 1
		};
	public:
		virtual void ClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void SetViewPort(int width, int height) = 0;
		virtual void DrawElements(std::shared_ptr<VertexArray>& vertexArray) = 0;

		static API GetAPI() { return s_API; }
		static RenderAPI* Get();
	private:
		static API s_API;
	};
}