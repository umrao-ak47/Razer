#pragma once

#include "Razer/Core.h"
#include "Razer/Window.h"
#include "Razer/LayerStack.h"
#include "Razer/imgui/ImguiLayer.h"
#include "Razer/Renderer/Shader.h"
#include "Razer/Renderer/Buffer.h"

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

		unsigned int m_VertexArray;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;

		static Application* s_Instance;
	};

	// To be implemented in client side
	Application* CreateApplication();
}


