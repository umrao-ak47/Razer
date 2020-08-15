#pragma once

#include "Razer/Core.h"

namespace rz {
	enum RZAPI class RenderAPI {
		None = 0,
		OpenGL = 1
	};

	class RZAPI Renderer {
	public:
		static RenderAPI GetAPI() { return s_RenderAPI; }
		static void SetAPI(RenderAPI api) { s_RenderAPI = api;  }
	private:
		static RenderAPI s_RenderAPI;
	};
}