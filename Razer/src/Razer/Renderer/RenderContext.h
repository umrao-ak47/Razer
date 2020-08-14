#pragma once

#include "Razer/Core.h"

namespace rz {
	class RZAPI RenderContext {
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}