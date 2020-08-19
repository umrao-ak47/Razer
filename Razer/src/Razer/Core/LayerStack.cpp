#include "RZPCH.h"
#include "LayerStack.h"

namespace rz{
	LayerStack::LayerStack() 
		:m_LayerInsertIndex(0) {}

	LayerStack::~LayerStack() {
		
	}

	void LayerStack::PushLayer(const Ref<Layer>& layer) {
		m_Layers.emplace(m_Layers.begin()+m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(const Ref<Layer>& overlay) {
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(const Ref<Layer>& layer) {
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end()) {
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(const Ref<Layer>& overlay) {
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end()) {
			m_Layers.erase(it);
		}
	}
}