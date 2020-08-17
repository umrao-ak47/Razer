#include "Razer/RZPCH.h"
#include "Razer.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <iostream>

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
			uniform mat4 u_ProjectionViewMatrix;
			uniform mat4 u_Trans;
			
			out vec3 v_Position;
			out vec4 v_Color;
			
			void main(){
				gl_Position = u_ProjectionViewMatrix * ( u_Trans * vec4(a_Position, 1.0));
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
			-0.75f, -0.75f, 0.0f, 0.0f, 0.0f,
			0.75f, -0.75f, 0.0f, 1.0f, 0.0f,
			0.75f, 0.75f, 0.0f, 1.0f, 1.0f,
			-0.75f, 0.75f, 0.0f, 0.0f, 1.0f
		};

		// shader code
		std::string squareVertSrc = R"(
			#version 410 core
			
			layout(location=0) in vec3 a_Position;
			layout(location=1) in vec2 a_TexCords;
			uniform mat4 u_ProjectionViewMatrix;
			
			out vec2 v_TexCords;
			
			void main(){
				gl_Position =  u_ProjectionViewMatrix * vec4(a_Position, 1.0);
				v_TexCords = a_TexCords;
			}
		)";

		std::string squareFragSrc = R"(
			#version 410 core
			
			layout(location=0) out vec4 o_Color;
			in vec2 v_TexCords;
			
			uniform sampler2D u_Texture;
			
			void main(){
				o_Color = texture(u_Texture, v_TexCords);
			}
		)";
		m_SquareShader = std::shared_ptr<Shader>(Shader::Create(squareVertSrc, squareFragSrc));

		std::shared_ptr<VertexBuffer> squareVB = std::shared_ptr<VertexBuffer>(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout(m_SquareShader->ExtractLayout());

		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int squareIndices[] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB = std::shared_ptr<IndexBuffer>(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(unsigned int)));
		m_SquareVA->SetIndexBuffer(squareIB);
		
		// set color of square
		m_SquareColor = glm::vec4(0.2f, 0.4f, 0.6f, 1.0f);
		m_Texture = std::shared_ptr<Texture>(Texture::Create("D://imgs//deadpool_refrence.png", true));

		m_Camera = Camera();
		m_Camera.SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
		//m_Camera.SetRotation(90.0f);

		m_Time = 0.0f;
	}

	// DEBUG MAT4
	static void Print(const glm::mat4& m) {
		for (int i = 0;i < 4; i++){
			for (int j = 0; j < 4; j++) {
				std::cout << m[i][j] << ' ';
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	void OnUpdate(float delta) override {
		//RZ_TRACE("FPS: {0}", 1.0 / delta);
		m_Time += delta;

		m_Trans = glm::mat4(1.0f);
		m_Trans = glm::translate(m_Trans, glm::vec3(0.5f * glm::cos(glm::radians(m_Time * 50.0f)), 0.5f * glm::sin(glm::radians(m_Time * 50.0f)), 0.0f));
		m_Trans = glm::rotate(m_Trans, glm::radians(m_Time * 50.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		m_Shader->Bind();
		m_Shader->UploadUniform("u_Trans", m_Trans);

		RendererCommand::ClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		Renderer::BeginScene(m_Camera);
		Renderer::Submit(m_SquareShader, m_SquareVA, m_Texture);
		Renderer::Submit(m_Shader, m_VertexArray);
		Renderer::EndScene();
	}

	void OnImguiRender() override {
		// Control Color of Saure with ImGui
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Event& e) override {
		//RZ_TRACE("ExampleLayer:: {0}", e);
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressEvent>(std::bind(&ExampleLayer::OnKeyPressEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseScrollEvent>(std::bind(&ExampleLayer::OnMouseScrollEvent, this, std::placeholders::_1));
	}

	bool OnKeyPressEvent(KeyPressEvent& e) {
		int keyCode = e.GetKeyCode();
		if (keyCode == RZ_KEY_UP) {
			m_Camera.SetPosition(m_Camera.GetPosition() + glm::vec3(0.0f, -1.0f, 0.0f));
		}
		else if (keyCode == RZ_KEY_DOWN) {
			m_Camera.SetPosition(m_Camera.GetPosition() + glm::vec3(0.0f, 1.0f, 0.0f));
		}
		if (keyCode == RZ_KEY_LEFT) {
			m_Camera.SetPosition(m_Camera.GetPosition() + glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (keyCode == RZ_KEY_RIGHT) {
			m_Camera.SetPosition(m_Camera.GetPosition() + glm::vec3(-1.0f, 0.0f, 0.0f));
		}
		
		return true;
	}

	bool OnMouseScrollEvent(MouseScrollEvent& e) {
		int z = e.GetYOffset();
		m_Camera.SetPosition(m_Camera.GetPosition() + glm::vec3(0.0f, 0.0f, -z));

		return true;
	}
private:
	Camera m_Camera;
	glm::vec4 m_SquareColor;
	glm::mat4 m_Trans;
	float m_Time;

	std::shared_ptr<Texture> m_Texture;
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