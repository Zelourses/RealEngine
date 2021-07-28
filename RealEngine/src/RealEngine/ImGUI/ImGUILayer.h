#pragma once

#include "RealEngine/Layer.h"
#include "RealEngine/Events/ApplicationEvent.h"
#include "RealEngine/Events/KeyEvent.h"
#include "RealEngine/Events/MouseEvent.h"

namespace RealEngine {
	class ImGUILayer : public Layer{
	public:
		ImGUILayer();
		~ImGUILayer();

		void onAttach() override;
		void onDetach() override;

		void onImGUIRender() override;

		void begin();
		void end();
		
	private:
		float frameTime = 0.0f;
	};
}

