#include "RZPCH.h"
#include "Renderer.h"


namespace rz {
	void Renderer::BeginScene() {

	}

	void Renderer::EndScene() {

	}

	void Renderer::Submit(std::shared_ptr<VertexArray>& vertexArray) {
		vertexArray->Bind();
		RendererCommand::DrawElements(vertexArray);
	}
}