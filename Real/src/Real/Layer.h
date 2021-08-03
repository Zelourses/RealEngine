#pragma once
#include "repch.h"

#include "Real/Core.h"
#include "Real/Events/Event.h"
#include "Real/Core/Timestep.h"

namespace Real {
	class Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();
		
		virtual void onAttach() {}
		virtual void onDetach() {}

		virtual void onImGUIRender() {}
		
		virtual void onUpdate(Timestep timestep) {}
		virtual void onEvent(Event& event) {}

		inline const std::string& getName() const { return debugName; }//May be getDebugName?

	protected:
		std::string debugName;
		
	};
}
