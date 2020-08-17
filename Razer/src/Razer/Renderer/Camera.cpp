#include "RZPCH.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace rz {
	Camera::Camera()
		: m_Position(0.0f, 0.0f, -1.0f),
		m_Rotation(0.0f),
		m_ProjectionMatrix(glm::perspectiveFov(glm::radians(45.0f), 1280.0f, 720.0f, 0.0f, 100.0f)),
		m_ViewMatrix(1.0f) {
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	
	void Camera::ReCalculateProjectionViewMatrix() {
		glm::mat4 trans = glm::translate(glm::mat4(1.0f), m_Position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		m_ViewMatrix = glm::inverse(trans);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}
