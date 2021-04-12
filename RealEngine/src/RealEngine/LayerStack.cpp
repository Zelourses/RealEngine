#include "repch.h"
#include "LayerStack.h"

namespace RealEngine{
	LayerStack::LayerStack() {
		layerInsert = layers.begin();
	}

	LayerStack::~LayerStack() {
		for (Layer* val : layers) {
			delete val;
		}
	}

	void LayerStack::pushLayer(Layer* layer) {
		layerInsert = layers.emplace(layerInsert,layer );
		++layerInsert;
	}

	void LayerStack::pushOverlay(Layer* overlay) {
		layers.emplace_back(overlay);
	}

	void LayerStack::popLayer(Layer* layer) {
		auto it = std::find(layers.begin(), layers.end(), layer);
		if (it != layers.end()) {
			layers.erase(it);
			--layerInsert;
		}
	}

	void LayerStack::popOverlay(Layer* overlay) {
		auto it = std::find(layers.begin(), layers.end(), overlay);
		if (it !=layers.end()) {
			layers.erase(it);
		}
	}


}
