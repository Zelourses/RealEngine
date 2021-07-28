#pragma once
#include "repch.h"

#include "RealEngine/Core.h"
#include "Events/Event.h"

namespace RealEngine {
	class Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();
		
		virtual void onAttach() {}
		virtual void onDetach() {}

		virtual void onImGUIRender() {}
		
		virtual void onUpdate() {}
		virtual void onEvent(Event& event) {}

		inline const std::string& getName() const { return debugName; }//May be getDebugName?

	protected:
		std::string debugName;
		
	};
}
