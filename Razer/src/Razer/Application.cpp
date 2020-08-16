#include "RZPCH.h"
#include "Application.h"

#include "Razer/Events/ApplicationEvent.h"
#include "Razer/Events/KeyEvent.h"
#include "Razer/Events/MouseEvent.h"
#include "Razer/Log.h"
#include "Razer/Input.h"
#include "Razer/Renderer/Renderer.h"


namespace rz {
	Application* Application::s_Instance = nullptr;

	Application::Application()
	: m_Running(true) {
		RZ_CORE_ASSERT(s_Instance, "Application Already exists");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		m_ImguiLayer = std::shared_ptr<ImguiLayer>(new ImguiLayer());
		PushOverlay(m_ImguiLayer);
	}

	Application::~Application() {
		
	}

	void Application::OnEvent(Event& e) {
		// RZ_CORE_TRACE("Application:: {0}", e.ToString());

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
			for (const auto& layer : m_LayerStack) {
				layer->OnUpdate();
			}
			m_ImguiLayer->Begin();
			for (const auto& layer : m_LayerStack) {
				layer->OnImguiRender();
			}
			m_ImguiLayer->End();

			m_Window->OnUpdate();
			//RZ_CORE_INFO("Window:: Update");
		}
	}

	void Application::PushLayer(const std::shared_ptr<Layer>& layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(const std::shared_ptr<Layer>& overlay) {
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
}