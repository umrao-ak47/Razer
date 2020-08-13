#pragma once

#include "Core.h"
#include "Layer.h"

namespace rz {
	class RZAPI LayerStack {
		using Stack = std::vector<Layer*>;
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		Stack::iterator begin() { return m_Layers.begin(); }
		Stack::iterator end() { return m_Layers.end(); }
	private:
		Stack m_Layers;
		Stack::iterator m_LayerInsert;
	};
}
