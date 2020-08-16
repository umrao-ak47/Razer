#include "RZPCH.h"
#include "WindowsTime.h"

#include <GLFW/glfw3.h>

namespace rz {
	float WindowsTimer::GetTime() {
		m_Time = (float)glfwGetTime();
		return m_Time;
	}
}

