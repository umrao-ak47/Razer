#pragma once

#include "Razer/Core.h"
#include "Razer/Renderer/Shader.h"
#include <memory>

namespace rz {
	class RZAPI BasicShader {
	private:
		Ref<Shader> m_Shader;
	};
}