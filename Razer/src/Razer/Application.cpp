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

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type) {
			case ShaderDataType::None:        return GL_NONE;
			case ShaderDataType::BOOL:        return GL_BOOL;
			case ShaderDataType::CHAR:        return GL_BYTE;
			case ShaderDataType::UINT:        return GL_UNSIGNED_INT;
			case ShaderDataType::UINT2:       return GL_UNSIGNED_INT;
			case ShaderDataType::UINT3:       return GL_UNSIGNED_INT;
			case ShaderDataType::UINT4:       return GL_UNSIGNED_INT;
			case ShaderDataType::INT:         return GL_INT;
			case ShaderDataType::INT2:        return GL_INT;
			case ShaderDataType::INT3:        return GL_INT;
			case ShaderDataType::INT4:        return GL_INT;
			case ShaderDataType::FLOAT:       return GL_FLOAT;
			case ShaderDataType::FLOAT2:      return GL_FLOAT;
			case ShaderDataType::FLOAT3:      return GL_FLOAT;
			case ShaderDataType::FLOAT4:      return GL_FLOAT;
			case ShaderDataType::MAT2:        return GL_FLOAT;
			case ShaderDataType::MAT3:        return GL_FLOAT;
			case ShaderDataType::MAT4:        return GL_FLOAT;
		}
		RZ_CORE_ASSERT(false, "Unknown Shader Data Type");
		return 0;
	}

	Application::Application()
	: m_Running(true) {
		RZ_CORE_ASSERT(s_Instance, "Application Already exists");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		m_ImguiLayer = new ImguiLayer();
		PushOverlay(m_ImguiLayer);

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		};

		m_VertexBuffer = std::unique_ptr<VertexBuffer>(VertexBuffer::Create(vertices, sizeof(vertices)));
		{
			BufferLayout layout = {
			{ShaderDataType::FLOAT3, "a_Position"},
			{ShaderDataType::FLOAT4, "a_Color"},
			//{ShaderDataType::FLOAT3, "a_Normal"}
			};

			m_VertexBuffer->SetLayout(layout);
		}
		auto& layout = m_VertexBuffer->GetLayout();
		unsigned int index = 0;
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.Count, ShaderDataTypeToOpenGLBaseType(element.Type), 
				(element.Normalized ? GL_TRUE : GL_FALSE), layout.GetStride(), (const void*)element.Offset);
			index++;
		}

		unsigned int indices[3] = { 0, 1, 2 };
		m_IndexBuffer = std::unique_ptr<IndexBuffer>(IndexBuffer::Create(indices, 3));
		
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


		m_Shader = std::unique_ptr<Shader>(Shader::Create(vertSrc, fragSrc));
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
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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