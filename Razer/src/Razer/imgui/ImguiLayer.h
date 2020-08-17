#pragma once

#include "Razer/Core.h"
#include "Razer/Core/Layer.h"
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

		void OnImguiRender() override;

		void Begin();
		void End();
	private:
		float m_Time;
	};
}