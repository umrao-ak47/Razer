#include "RZPCH.h"
#include "WindowsWindow.h"
#include "Razer/Log.h"

#include  <glad/glad.h>

namespace rz {
	static bool s_GLFWinitiaized = false;

	void GLFWErrorCallback(int err, const char* msg) {
		RZ_CORE_ERROR("GLFW Error ({0}): {1}", err, msg);
	}

	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {
		Init(props);
	}

	WindowsWindow::~WindowsWindow() {
		ShutDown();
	}

	void WindowsWindow::Init(const WindowProps& props) {
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		RZ_CORE_INFO("Creating Window {0} ({1}, {2})", props.Title, props.Width, props.Height);
		if (!s_GLFWinitiaized) {
			int success = glfwInit();
			RZ_CORE_ASSERT(success, "Could not initialize GLFW");

			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWinitiaized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, props.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		RZ_CORE_ASSERT(status, "Failed to initialize Glad");
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// setting GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, 
			[](GLFWwindow* window, int width, int height) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			}
		);

		glfwSetWindowCloseCallback(m_Window,
			[](GLFWwindow* window) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				WindowCloseEvent event;
				data.EventCallback(event);
			}
		);

		glfwSetCharCallback(m_Window,
			[](GLFWwindow* window, unsigned int keycode) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyTypeEvent event(keycode);
				data.EventCallback(event);
			}
		);

		glfwSetKeyCallback(m_Window,
			[](GLFWwindow* window, int keycode, int scancode, int action, int mods) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action) {
					case GLFW_PRESS:
					{
						KeyPressEvent event(keycode, 0);
						data.EventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressEvent event(keycode, 1);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleaseEvent event(keycode);
						data.EventCallback(event);
						break;
					}
				}
			}
		);

		glfwSetMouseButtonCallback(m_Window,
			[](GLFWwindow* window, int button, int action, int mods) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action) {
					case GLFW_PRESS:
					{
						MouseButtonPressEvent event(button);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleaseEvent event(button);
						data.EventCallback(event);
						break;
					}
				}
			}
		);

		glfwSetScrollCallback(m_Window,
			[](GLFWwindow* window, double xOffset, double yOffset) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrollEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			}
		);

		glfwSetCursorPosCallback(m_Window,
			[](GLFWwindow* window, double x, double y) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMoveEvent event((float)x, (float)y);
				data.EventCallback(event);
			}
		);
	}

	void WindowsWindow::ShutDown() {
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled) {
		if (enabled) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}
		m_Data.VSync = enabled;
	}
}
