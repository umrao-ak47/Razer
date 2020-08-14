#include "Razer/RZPCH.h"
#include "Razer.h"

#include "imgui.h"

class ExampleLayer : public rz::Layer {
public:
	ExampleLayer()
		: rz::Layer("Example") {}

	void OnUpdate() override {
		//RZ_INFO("ExampleLayer:: Update");
		if (rz::Input::IsKeyPressed(RZ_KEY_TAB)) {
			RZ_WARN("TAB Key Pressed");
		}
	}

	void OnImguiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(rz::Event& e) override {
		//RZ_TRACE("ExampleLayer:: {0}", e.ToString());
	}

};

class App : public rz::Application {
public:
	App() {
		PushLayer(new ExampleLayer());
	}
	~App() {

	}
};

rz::Application* rz::CreateApplication() {
	return new App();
}