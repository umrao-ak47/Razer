#pragma once

#include "Razer/Core.h"
#include "Razer/Layer.h"
#include "Razer/Events/Event.h"
#include "Razer/Events/ApplicationEvent.h"
#include "Razer/Events/KeyEvent.h"
#include "Razer/Events/MouseEvent.h"

namespace rz {
	class RZAPI ImguiLayer : public Layer {
	public:
		ImguiLayer();
		~ImguiLayer();

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate() override;
		void OnEvent(Event& event) override;
	private:
		bool OnMouseButtonPressEvent(MouseButtonPressEvent& event);
		bool OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& event);
		bool OnMouseScrollEvent(MouseScrollEvent& event);
		bool OnMouseMoveEvent(MouseMoveEvent& event);
		bool OnKeyPressEvent(KeyPressEvent& event);
		bool OnKeyReleaseEvent(KeyReleaseEvent& event);
		bool OnKeyTypeEvent(KeyTypeEvent& event);
		bool OnWindowResizeEvent(WindowResizeEvent& event);
	private:
		float m_Time;
	};
}