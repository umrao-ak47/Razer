#pragma once

#include "Razer/Core.h"
#include "Layer.h"
#include <memory>

namespace rz {
	class RZAPI LayerStack {
		using Stack = std::vector<std::shared_ptr<Layer>>;
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(std::shared_ptr<Layer> layer);
		void PushOverlay(std::shared_ptr<Layer> overlay);
		void PopLayer(std::shared_ptr<Layer> layer);
		void PopOverlay(std::shared_ptr<Layer> overlay);

		Stack::iterator begin() { return m_Layers.begin(); }
		Stack::iterator end() { return m_Layers.end(); }
	private:
		Stack m_Layers;
		unsigned int m_LayerInsertIndex;
	};
}
