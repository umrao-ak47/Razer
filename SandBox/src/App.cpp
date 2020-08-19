#include "Razer/RZPCH.h"
#include "Razer.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace rz;

class ExampleLayer : public Layer {
public:
	ExampleLayer()
		: Layer("Example") {

		std::vector<float> vertices = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		};

		std::vector<unsigned int> indices = { 0, 1, 2 };

		m_Mesh = Mesh();
		m_Mesh.SetVertexData(vertices);
		m_Mesh.SetIndexData(indices);

		m_VertexArray = Ref<VertexArray>(VertexArray::Create());

		Ref<VertexBuffer> vertexBuffer = Ref<VertexBuffer>(VertexBuffer::Create(&m_Mesh.GetVertexData()[0], m_Mesh.GetVetexDataSize()));
		{
			BufferLayout layout = {
			{ShaderDataType::FLOAT3, "a_Position"},
			{ShaderDataType::FLOAT4, "a_Color"},
			//{ShaderDataType::FLOAT3, "a_Normal"}
			};

			vertexBuffer->SetLayout(layout);
		}
		
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		Ref<IndexBuffer> indexBuffer = Ref<IndexBuffer>(IndexBuffer::Create(&m_Mesh.GetIndexData()[0], m_Mesh.GetIndexCount()));
		m_VertexArray->SetIndexBuffer(indexBuffer);
		

		// shader code
		std::string vertSrc = R"(
			#version 410 core
			
			layout(location=0) in vec3 a_Position;
			layout(location=1) in vec4 a_Color;
			uniform mat4 u_Transformation;
			uniform mat4 u_ProjectionViewMatrix;
			
			out vec4 v_Color;
			
			void main(){
				gl_Position = u_ProjectionViewMatrix * (u_Transformation * vec4(a_Position, 1.0));
				v_Color = a_Color;
			}
		)";

		std::string fragSrc = R"(
			#version 410 core
			
			in vec4 v_Color;
			layout(location=0) out vec4 o_Color;
			
			void main(){
				o_Color = v_Color;
			}
		)";

		m_Shader = Ref<Shader>(Shader::Create(vertSrc, fragSrc));


		std::vector<float> squareVertices = {
			// front
			-0.75f, -0.75f, 0.75f, 0.0f, 0.0f, // 1
			0.75f, -0.75f, 0.75f, 1.0f, 0.0f, // 2
			0.75f, 0.75f, 0.75f, 1.0f, 1.0f,   // 3
			-0.75f, 0.75f, 0.75f, 0.0f, 1.0f,  // 4
			// back
			- 0.75f, -0.75f, -0.75f, 0.0f, 0.0f, // 5
			0.75f, -0.75f, -0.75f, 1.0f, 0.0f,  // 6
			0.75f, 0.75f, -0.75f, 1.0f, 1.0f,  // 7
			-0.75f, 0.75f, -0.75f, 0.0f, 1.0f  // 8
		};

		std::vector<unsigned int> squareIndices = {
			// front
			0, 1, 2, 2, 3, 0,
			// right
			1, 5, 6, 6, 2, 1,
			// back
			7, 6, 5, 5, 4, 7,
			// left
			4, 0, 3, 3, 7, 4,
			// bottom
			4, 5, 1, 1, 0, 4,
			// top
			3, 2, 6, 6, 7, 3
		};

		m_Square = Mesh();
		m_Square.SetVertexData(squareVertices);
		m_Square.SetIndexData(squareIndices);
		
		m_SquareVA = Ref<VertexArray>(VertexArray::Create());
		
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
			uniform vec3 u_LightColor;
			
			uniform sampler2D u_Texture;
			
			void main(){
				o_Color = vec4(u_LightColor, 1.0f) * texture(u_Texture, v_TexCords);
			}
		)";

		m_SquareShader = Ref<Shader>(Shader::Create(squareVertSrc, squareFragSrc));

		Ref<VertexBuffer> squareVB = Ref<VertexBuffer>(VertexBuffer::Create(&m_Square.GetVertexData()[0], m_Square.GetVetexDataSize()));
		squareVB->SetLayout(m_SquareShader->ExtractLayout());
		m_SquareVA->AddVertexBuffer(squareVB);

		Ref<IndexBuffer> squareIB = Ref<IndexBuffer>(IndexBuffer::Create(&m_Square.GetIndexData()[0], m_Square.GetIndexCount()));
		m_SquareVA->SetIndexBuffer(squareIB);

		m_Texture = Ref<Texture>(Texture::Create("D://imgs//deadpool_refrence.png", true));

		m_Camera = Camera();
		m_Camera.SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
		//m_Camera.SetRotation(90.0f);

		m_Time = 0.0f;
		m_Mesh.SetScale(glm::vec3(0.5f, 0.5f, 0.5f));

		// Light Calculations
		std::vector<float> lightVertices = {
			// front
			-0.75f, -0.75f, 0.75f, // 1
			0.75f, -0.75f, 0.75f, // 2
			0.75f, 0.75f, 0.75f,  // 3
			-0.75f, 0.75f, 0.75f,  // 4
			// back
			-0.75f, -0.75f, -0.75f, // 5
			0.75f, -0.75f, -0.75f,   // 6
			0.75f, 0.75f, -0.75f,   // 7
			-0.75f, 0.75f, -0.75f,  // 8
		};

		m_LightSource = Mesh();
		m_LightSource.SetVertexData(lightVertices);
		m_LightSource.SetIndexData(squareIndices);
		m_LightSource.SetPoistion(glm::vec3(2.0f, 0.0f, 0.0f));
		m_LightSource.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

		m_LightVA = Ref<VertexArray>(VertexArray::Create());

		// shader code
		std::string lightVertSrc = R"(
			#version 410 core
			
			layout(location=0) in vec3 a_Position;
			uniform mat4 u_Transformation;
			uniform mat4 u_ProjectionViewMatrix;
			
			void main(){
				gl_Position =  u_ProjectionViewMatrix * (u_Transformation * vec4(a_Position, 1.0));
			}
		)";

		std::string lightFragSrc = R"(
			#version 410 core
			
			layout(location=0) out vec4 o_Color;
			
			void main(){
				o_Color = vec4(1.0f);
			}
		)";

		m_LightShader = Ref<Shader>(Shader::Create(lightVertSrc, lightFragSrc));

		Ref<VertexBuffer> lightVB = Ref<VertexBuffer>(VertexBuffer::Create(&m_LightSource.GetVertexData()[0], m_LightSource.GetVetexDataSize()));
		lightVB->SetLayout(m_LightShader->ExtractLayout());
		m_LightVA->AddVertexBuffer(lightVB);

		Ref<IndexBuffer> lightIB = Ref<IndexBuffer>(IndexBuffer::Create(&m_LightSource.GetIndexData()[0], m_LightSource.GetIndexCount()));
		m_LightVA->SetIndexBuffer(lightIB);

		m_LightShader->Bind();
		m_LightShader->UploadUniform("u_Transformation", m_LightSource.GetModelMatrix());

		// upload light color
		m_SquareShader->Bind();
		m_SquareShader->UploadUniform("u_LightColor", glm::vec3(1.0f, 1.0f, 1.0f));
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

		m_Mesh.SetPoistion(glm::vec3(0.5f * glm::cos(glm::radians(m_Time * 50.0f)), 0.5f * glm::sin(glm::radians(m_Time * 50.0f)), 0.0f));
		m_Mesh.SetRotation(glm::vec3(0.0f, 0.0f, glm::radians(m_Time * 50.f)));
		m_Shader->Bind();
		m_Shader->UploadUniform("u_Transformation", m_Mesh.GetModelMatrix());

		RendererCommand::ClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		Renderer::BeginScene(m_Camera);
		Renderer::Submit(m_SquareShader, m_SquareVA, m_Texture);
		Renderer::Submit(m_LightShader, m_LightVA);
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
		float z = e.GetYOffset();
		m_Camera.SetPosition(m_Camera.GetPosition() + glm::vec3(0.0f, 0.0f, -z));

		return true;
	}
private:
	Camera m_Camera;
	glm::vec4 m_SquareColor;
	glm::mat4 m_Trans;
	float m_Time;

	Mesh m_Mesh;
	Mesh m_Square;
	Mesh m_LightSource;

	Ref<Texture> m_Texture;
	Ref<VertexArray> m_VertexArray;
	Ref<Shader> m_Shader;
	
	Ref<VertexArray> m_SquareVA;
	Ref<Shader> m_SquareShader;

	Ref<VertexArray> m_LightVA;
	Ref<Shader> m_LightShader;
};




// Actual App

class App : public Application {
public:
	App() {
		PushLayer(Ref<ExampleLayer>(new ExampleLayer()));
	}
	~App() {

	}
};

Application* rz::CreateApplication() {
	return new App();
}