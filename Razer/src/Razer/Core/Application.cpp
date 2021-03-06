#include "RZPCH.h"
#include "Application.h"

#include "Razer/Events/ApplicationEvent.h"
#include "Razer/Events/KeyEvent.h"
#include "Razer/Events/MouseEvent.h"
#include "Razer/Core/Log.h"
#include "Razer/Core/Input.h"
#include "Razer/Renderer/Renderer.h"


namespace rz {
	Application* Application::s_Instance = nullptr;

	Application::Application()
	: m_Running(true) {
		RZ_CORE_ASSERT(!s_Instance, "Application Already exists");
		s_Instance = this;

		m_Window = Scope<Window>(Window::Create());
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		m_ImguiLayer = Ref<ImguiLayer>(new ImguiLayer());
		PushOverlay(m_ImguiLayer);

		m_Timer = std::unique_ptr<Timer>(Timer::Get());
	}

	Application::~Application() {
		
	}

	void Application::OnEvent(Event& e) {
		//RZ_CORE_TRACE("Application:: {0}", e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Application::OnWindowResize, this, std::placeholders::_1));
		if (e.Handled()) return;

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*(--it))->OnEvent(e);
			if (e.Handled()) {
				break;
			}
		}
	}

	void Application::Run() {
		float lastTime = m_Timer->GetTime();
		while (m_Running) {
			float time = m_Timer->GetTime();
			float delta = time - lastTime;
			lastTime = time;
			for (const auto& layer : m_LayerStack) {
				layer->OnUpdate(delta);
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

	void Application::PushLayer(const Ref<Layer>& layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(const Ref<Layer>& overlay) {
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e) {
		RendererCommand::SetViewPort(e.GetWidth(), e.GetHeight());
		return true;
	}
}