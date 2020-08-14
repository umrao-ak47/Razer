#include "RZPCH.h"
#include "OpenGLContext.h"

#include "Razer/Log.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace rz {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle) {
		RZ_CORE_ASSERT(windowHandle, "Window Handle is null");
	}

	void OpenGLContext::Init() {
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		RZ_CORE_ASSERT(status, "Failed to initialize Glad");

		RZ_CORE_INFO("OpenGL Info::");
		RZ_CORE_INFO("    Vendor: {0}", glGetString(GL_VENDOR));
		RZ_CORE_INFO("    Renderer: {0}", glGetString(GL_RENDERER));
		RZ_CORE_INFO("    Version: {0}", glGetString(GL_VERSION));
	}
	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(m_WindowHandle);
	}
}