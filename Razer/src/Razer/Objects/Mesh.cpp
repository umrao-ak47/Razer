#include "RZPCH.h"
#include "Mesh.h"

namespace rz {
	void Mesh::RecalculateModelMatrix() {
		m_ModelMatrix = glm::translate(glm::mat4(1.0f), m_Position);
		m_ModelMatrix = glm::rotate(m_ModelMatrix, m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		m_ModelMatrix = glm::scale(m_ModelMatrix, m_Scale);
	}
}

