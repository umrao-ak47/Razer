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

		m_ImguiLayer = new ImguiLayer();
		PushOverlay(m_ImguiLayer);

		m_VertexArray = std::shared_ptr<VertexArray>(VertexArray::Create());

		float vertices[] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		};

		std::shared_ptr<VertexBuffer> vertexBuffer = std::shared_ptr<VertexBuffer>(VertexBuffer::Create(vertices, sizeof(vertices)));
		{
			BufferLayout layout = {
			{ShaderDataType::FLOAT3, "a_Position"},
			{ShaderDataType::FLOAT4, "a_Color"},
			//{ShaderDataType::FLOAT3, "a_Normal"}
			};

			vertexBuffer->SetLayout(layout);
		}

		m_VertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer = std::shared_ptr<IndexBuffer>(IndexBuffer::Create(indices, sizeof(indices)/sizeof(unsigned int)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		// shader code
		std::string vertSrc = R"(
			#version 410 core
			
			layout(location=0) in vec3 a_Position;
			layout(location=1) in vec4 a_Color;
			out vec3 v_Position;
			out vec4 v_Color;
			
			void main(){
				gl_Position = vec4(a_Position, 1.0);
				v_Position = a_Position;
				v_Color = a_Color;
			}
		)";

		std::string fragSrc = R"(
			#version 410 core
			
			in vec3 v_Position;
			in vec4 v_Color;
			layout(location=0) out vec4 o_Color;
			
			void main(){
				o_Color = vec4(v_Position * 0.5f + 0.5f , 1.0f);
				o_Color = v_Color;
			}
		)";

		m_Shader = std::shared_ptr<Shader>(Shader::Create(vertSrc, fragSrc));

		m_SquareVA = std::shared_ptr<VertexArray>(VertexArray::Create());
		float squareVertices[] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f,
		};

		std::shared_ptr<VertexBuffer> squareVB = std::shared_ptr<VertexBuffer>(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		{
			BufferLayout layout = {
				{ShaderDataType::FLOAT3, "a_Position"}
			};

			squareVB->SetLayout(layout);
		}

		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int squareIndices[] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB = std::shared_ptr<IndexBuffer>(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(unsigned int)));
		m_SquareVA->SetIndexBuffer(squareIB);

		// shader code
		std::string squareVertSrc = R"(
			#version 410 core
			
			layout(location=0) in vec3 a_Position;
			
			void main(){
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

		std::string squareFragSrc = R"(
			#version 410 core
			
			layout(location=0) out vec4 o_Color;
			
			void main(){
				o_Color = vec4(0.3f, 0.3f, 0.8f, 1.0f);
			}
		)";
		m_SquareShader = std::shared_ptr<Shader>(Shader::Create(squareVertSrc, squareFragSrc));
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
			RendererCommand::ClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
			Renderer::BeginScene();
			m_SquareShader->Bind();
			Renderer::Submit(m_SquareVA);

			m_Shader->Bind();
			Renderer::Submit(m_VertexArray);

			Renderer::EndScene();
			
			
			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}
			m_ImguiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImguiRender();
			}
			m_ImguiLayer->End();

			m_Window->OnUpdate();
			//RZ_CORE_INFO("Window:: Update");
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