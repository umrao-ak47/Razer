#include "RZPCH.h"
#include "Application.h"

#include "Razer/Events/ApplicationEvent.h"
#include "Razer/Events/KeyEvent.h"
#include "Razer/Events/MouseEvent.h"
#include "Razer/Log.h"
#include "Razer/Input.h"
#include <glad/glad.h>


namespace rz {
	Application* Application::s_Instance = nullptr;

	Application::Application()
	: m_Running(true) {
		RZ_CORE_ASSERT(s_Instance, "Application Already exists");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	}

	Application::~Application() {
		// nothing fancy
	}

	void Application::OnEvent(Event& e) {
		RZ_CORE_TRACE("Application:: {0}", e.ToString());

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));

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

			float x = Input::GetMouseX();
			float y = Input::GetMouseY();
			RZ_CORE_TRACE("{0}, {1}", x, y);

			m_Window->OnUpdate();
			RZ_CORE_INFO("Window:: Update");
		}
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
}