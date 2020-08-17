#pragma once

#include "Razer/Core.h"
#include "Razer/Events/Event.h"

namespace rz {
	class RZAPI Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float delta) {}
		virtual void OnImguiRender() {}
		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return m_DebugName;  }

	protected:
		std::string m_DebugName;
	};
}


