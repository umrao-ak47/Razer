#pragma once

#include "Razer/Core.h"
#include "Razer/Core/Window.h"
#include "Razer/Core/LayerStack.h"
#include "Razer/imgui/ImguiLayer.h"
#include "Razer/Core/Time.h"

namespace rz {
	class RZAPI Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(const std::shared_ptr<Layer>& layer);
		void PushOverlay(const std::shared_ptr<Layer>& overlay);

		Window& GetWindow() { return *m_Window; }
		static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		std::unique_ptr<Timer> m_Timer;
		std::shared_ptr<ImguiLayer> m_ImguiLayer;
		std::unique_ptr<Window> m_Window;
		bool m_Running;
		LayerStack m_LayerStack;

		static Application* s_Instance;
	};

	// To be implemented in client side
	Application* CreateApplication();
}


