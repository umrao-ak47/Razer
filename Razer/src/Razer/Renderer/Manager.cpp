#include "RZPCH.h"
#include "Manager.h"


namespace rz {
	unsigned int ShaderManager::Create(const std::string& vertSrc, const std::string& fragSrc)
	{
		Ref<Shader> shader = Ref<Shader>(Shader::Create(vertSrc, fragSrc));
		m_Objects.push_back(shader);
		return m_Objects.size() - 1;
	}
}

