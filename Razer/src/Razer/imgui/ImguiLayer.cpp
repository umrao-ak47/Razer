#include "RZPCH.h"
#include "ImguiLayer.h"

#include "Platform/OpenGL/ImguiRenderer.h"
#include "Razer/Application.h"
#include "Razer/Log.h"

#include <imgui.h>
// temporary
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace rz {
	ImguiLayer::ImguiLayer()
		: Layer("ImguiLayer") {
		// nothing fancy
	}

	ImguiLayer::~ImguiLayer() {
		// nothing fancy
	}

	void ImguiLayer::OnAttach() {
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        // Replace with our key biindings
        // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
        io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
        io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
        io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
        io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
        io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
        io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

        // init opengl
        ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImguiLayer::OnDetach() {

	}

	void ImguiLayer::OnUpdate() {
        RZ_CORE_INFO("ImguiLayer:: Update");

        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        float time = (float)glfwGetTime();
        io.DeltaTime = (m_Time > 0.0f) ? (time - m_Time) : (1.0f / 60.0f);
        m_Time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImguiLayer::OnEvent(Event& event) {
        RZ_CORE_TRACE("ImguiLayer:: {0}", event.ToString());

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseButtonPressEvent>(std::bind(&ImguiLayer::OnMouseButtonPressEvent, this, std::placeholders::_1));
        dispatcher.Dispatch<MouseButtonReleaseEvent>(std::bind(&ImguiLayer::OnMouseButtonReleaseEvent, this, std::placeholders::_1));
        dispatcher.Dispatch<MouseScrollEvent>(std::bind(&ImguiLayer::OnMouseScrollEvent, this, std::placeholders::_1));
        dispatcher.Dispatch<MouseMoveEvent>(std::bind(&ImguiLayer::OnMouseMoveEvent, this, std::placeholders::_1));
        dispatcher.Dispatch<KeyPressEvent>(std::bind(&ImguiLayer::OnKeyPressEvent, this, std::placeholders::_1));
        dispatcher.Dispatch<KeyReleaseEvent>(std::bind(&ImguiLayer::OnKeyReleaseEvent, this, std::placeholders::_1));
        dispatcher.Dispatch<KeyTypeEvent>(std::bind(&ImguiLayer::OnKeyTypeEvent, this, std::placeholders::_1));
        dispatcher.Dispatch<WindowResizeEvent>(std::bind(&ImguiLayer::OnWindowResizeEvent, this, std::placeholders::_1));
	}

    bool ImguiLayer::OnMouseButtonPressEvent(MouseButtonPressEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[event.GetMouseButton()] = true;

        return false;
    }

    bool ImguiLayer::OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[event.GetMouseButton()] = false;

        return false;
    }

    bool ImguiLayer::OnMouseScrollEvent(MouseScrollEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheelH += event.GetXOffset();
        io.MouseWheel += event.GetYOffset();

        return false;
    }

    bool ImguiLayer::OnMouseMoveEvent(MouseMoveEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(event.GetX(), event.GetY());

        return false;
    }

    bool ImguiLayer::OnKeyPressEvent(KeyPressEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[event.GetKeyCode()] = true;

        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
        
        return false;
    }

    bool ImguiLayer::OnKeyReleaseEvent(KeyReleaseEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[event.GetKeyCode()] = false;

        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

        return false;
    }

    bool ImguiLayer::OnKeyTypeEvent(KeyTypeEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        int keycode = event.GetKeyCode();
        if (keycode > 0 && keycode < 0x10000) {
            io.AddInputCharacter((unsigned short)keycode);
        }
        return false;
    }

    bool ImguiLayer::OnWindowResizeEvent(WindowResizeEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(event.GetWidth(), event.GetHeight());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        glViewport(0, 0, event.GetWidth(), event.GetHeight());
        return false;
    }
}