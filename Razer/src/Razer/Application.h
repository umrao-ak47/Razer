#pragma once

#include "Razer/Core.h"
#include "Razer/Window.h"
#include "Razer/LayerStack.h"
#include "Razer/imgui/ImguiLayer.h"

namespace rz {
	class RZAPI Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		Window& GetWindow() { return *m_Window; }
		static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		ImguiLayer* m_ImguiLayer;
		std::unique_ptr<Window> m_Window;
		bool m_Running;
		LayerStack m_LayerStack;

		static Application* s_Instance;
	};

	// To be implemented in client side
	Application* CreateApplication();
}


