#pragma once

#include "Razer/Core.h"

#include "Razer/Renderer/Array.h"
#include "Razer/Renderer/Shader.h"

namespace rz {
	class RZAPI VertexArrayManger {
	public:
		VertexArrayManger() {}
		~VertexArrayManger() {}
	private:
		static std::vector<Ref<VertexArray>> m_Ojects;
	};
}