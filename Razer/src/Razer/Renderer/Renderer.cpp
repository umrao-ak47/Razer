#include "RZPCH.h"
#include "Renderer.h"


namespace rz {
	Renderer::SceneData* Renderer::m_Data = new Renderer::SceneData();

	void Renderer::BeginScene(const Camera& camera) {
		m_Data->ProjectionViewMatrix = camera.GetProjectionViewMatrix();
	}

	void Renderer::EndScene() {

	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray) {
		shader->Bind();
		shader->UploadUniform("u_ProjectionViewMatrix", m_Data->ProjectionViewMatrix);
		vertexArray->Bind();
		RendererCommand::DrawElements(vertexArray);
	}
}