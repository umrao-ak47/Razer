#pragma once

#include "Razer/Core.h"
#include <glm/glm.hpp>


namespace rz{
	class RZAPI Camera {
	public:
		Camera();

		const glm::vec3& GetPosition() const { return m_Position; }
		float GetRotation() { return m_Rotation; }

		void SetPosition(const glm::vec3& position) { m_Position = position; ReCalculateProjectionViewMatrix(); }
		void SetRotation(float rotation) { m_Rotation = rotation; ReCalculateProjectionViewMatrix(); }

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetProjectionViewMatrix() const { return m_ProjectionViewMatrix; }
	private:
		void ReCalculateProjectionViewMatrix();
	private:
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ProjectionViewMatrix;
		glm::vec3 m_Position;
		float m_Rotation;
	};
}