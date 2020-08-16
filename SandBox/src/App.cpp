#include "Razer/RZPCH.h"
#include "Razer.h"

#include "imgui.h"
using namespace rz;

class ExampleLayer : public Layer {
public:
	ExampleLayer()
		: Layer("Example") {
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
		std::shared_ptr<IndexBuffer> indexBuffer = std::shared_ptr<IndexBuffer>(IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
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
			-0.75f, -0.75f, 0.0f,
			0.75f, -0.75f, 0.0f,
			0.75f, 0.75f, 0.0f,
			-0.75f, 0.75f, 0.0f,
		};

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

		std::shared_ptr<VertexBuffer> squareVB = std::shared_ptr<VertexBuffer>(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout(m_SquareShader->ExtractLayout());

		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int squareIndices[] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB = std::shared_ptr<IndexBuffer>(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(unsigned int)));
		m_SquareVA->SetIndexBuffer(squareIB);
	}

	void OnUpdate() override {
		RendererCommand::ClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		Renderer::BeginScene();
		m_SquareShader->Bind();
		Renderer::Submit(m_SquareVA);

		m_Shader->Bind();
		Renderer::Submit(m_VertexArray);

		Renderer::EndScene();
	}

	void OnImguiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(Event& e) override {
		//RZ_TRACE("ExampleLayer:: {0}", e.ToString());
	}
private:
	std::shared_ptr<VertexArray> m_VertexArray;
	std::shared_ptr<Shader> m_Shader;

	std::shared_ptr<VertexArray> m_SquareVA;
	std::shared_ptr<Shader> m_SquareShader;
};




// Actual App

class App : public Application {
public:
	App() {
		PushLayer(std::shared_ptr<ExampleLayer>(new ExampleLayer()));
	}
	~App() {

	}
};

Application* rz::CreateApplication() {
	return new App();
}