#include "RZPCH.h"
#include "OpenGLRenderAPI.h"

#include <glad/glad.h>

namespace rz {
	void OpenGLRenderAPI::ClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRenderAPI::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRenderAPI::SetViewPort(int width, int height) {
		const float ar = (float)width / (float)height;

		glViewport(0, 0, width, height);
		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();
		//glFrustum(-ar, ar, -1.0f, 1.0f, -1.0f, 100.0f);

		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
	}

	void OpenGLRenderAPI::DrawElements(std::shared_ptr<VertexArray>& vertexArray) {
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

}