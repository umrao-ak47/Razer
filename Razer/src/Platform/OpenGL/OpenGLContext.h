#pragma once

#include "Razer/Renderer/RenderContext.h"

struct GLFWwindow;;

namespace rz{
	class OpenGLContext : public RenderContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		void Init() override;
		void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}