#pragma once
#include "repch.h"

#include "RealEngine/Core.h"
#include "RealEngine/Events/Event.h"
#include "RealEngine/Core/Timestep.h"

namespace RealEngine {
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
