#pragma once

#include "Razer/Core.h"
#include "Razer/Core/Layer.h"
#include <vector>

namespace rz {
	class RZAPI LayerStack {
		using Stack = std::vector<Ref<Layer>>;
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(const Ref<Layer>& layer);
		void PushOverlay(const Ref<Layer>& overlay);
		void PopLayer(const Ref<Layer>& layer);
		void PopOverlay(const Ref<Layer>& overlay);

		Stack::iterator begin() { return m_Layers.begin(); }
		Stack::iterator end() { return m_Layers.end(); }
	private:
		Stack m_Layers;
		unsigned int m_LayerInsertIndex;
	};
}
