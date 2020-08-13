#include "RZPCH.h"
#include "Application.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Log.h"

#include <GLFW/glfw3.h>

namespace rz {
	Application::Application()
	: m_Running(true) {
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	}

	Application::~Application() {
		// nothing fancy
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
		
		RZ_CORE_TRACE(e.ToString());

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*(--it))->OnEvent(e);
			if (e.Handled()) {
				break;
			}
		}
	}

	void Application::Run() {
		while (m_Running) {
			glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			m_Window->OnUpdate();
		}
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
}