#pragma once

#include "Razer/Core.h"
#include "Razer/Renderer/Array.h"
#include "Razer/Renderer/Buffer.h"
#include "Razer/Renderer/Shader.h"
#include "Razer/Renderer/Texture.h"
#include <memory>
#include <vector>

namespace rz {
	class RZAPI Mesh {
	public:
		Mesh()
			: m_Position(0.0f), m_Rotation(0.0f),
			m_Scale(1.0f), m_ModelMatrix(1.0f) {}

		~Mesh() {}

		const std::vector<float>& GetVertexData() const { return m_Verticies; }
		const std::vector<unsigned int>& GetIndexData() const { return m_Indicies; }

		void SetVertexData(std::vector<float> verticies) { m_Verticies = verticies; }
		void SetIndexData(std::vector<unsigned int> indicies) { m_Indicies = indicies; }

		const glm::mat4& GetModelMatrix() const { return m_ModelMatrix; }

		const glm::vec3& GetPoistion() const { return m_Position; }
		const glm::vec3& GetRotation() const { return m_Rotation; }

		void SetPoistion(glm::vec3 position) { m_Position = position; RecalculateModelMatrix(); }
		void SetRotation(glm::vec3 rotation) { m_Rotation = rotation; RecalculateModelMatrix(); }
		void SetScale(glm::vec3 scale) { m_Scale = scale; RecalculateModelMatrix(); }

		unsigned int GetVetexDataSize() const { return static_cast<unsigned int>(m_Verticies.size() * sizeof(float)); }
		unsigned int GetIndexCount() const { return static_cast<unsigned int>(m_Indicies.size()); }
	private:
		void RecalculateModelMatrix();
	private:
		std::vector<float> m_Verticies;
		std::vector<unsigned int> m_Indicies;
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
		glm::mat4 m_ModelMatrix;
	};
}