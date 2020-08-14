#include "RZPCH.h"
#include "WindowsInput.h"

#include "Razer/Application.h"
#include <GLFW/glfw3.h>

namespace rz {
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int keyCode)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetKey(window, keyCode);
		return ((state == GLFW_PRESS) || (state == GLFW_REPEAT));
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button) {
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetMouseButton(window, button);
		return (state == GLFW_PRESS);
	}

	std::pair<float, float> WindowsInput::GetMousePosImpl() {
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return std::make_pair((float)xPos, (float)yPos);
	}

	float WindowsInput::GetMouseXImpl() {
		return GetMousePosImpl().first;
	}

	float WindowsInput::GetMouseYImpl() {
		return GetMousePosImpl().second;
	}
}