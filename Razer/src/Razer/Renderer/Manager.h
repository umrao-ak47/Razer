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


	class RZAPI ShaderManager {
	public:
		ShaderManager() {}
		~ShaderManager() {}

		static unsigned int Create(const std::string& vertSrc, const std::string& fragSrc);

		static Ref<Shader> Get(unsigned int index) { return m_Objects.at(index); }
	private:
		static std::vector<Ref<Shader>> m_Objects;
	};
}