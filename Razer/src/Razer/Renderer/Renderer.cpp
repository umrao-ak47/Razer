#include "RZPCH.h"
#include "Renderer.h"


namespace rz {
	Renderer::SceneData* Renderer::m_Data = new Renderer::SceneData();

	void Renderer::BeginScene(const Camera& camera) {
		m_Data->ProjectionViewMatrix = camera.GetProjectionViewMatrix();
	}

	void Renderer::EndScene() {

	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, 
		const Ref<Texture>& texture) {
		shader->Bind();
		shader->UploadUniform("u_ProjectionViewMatrix", m_Data->ProjectionViewMatrix);
		if (texture != nullptr) { texture->Bind(); }
		vertexArray->Bind();
		RendererCommand::DrawElements(vertexArray);
	}
}