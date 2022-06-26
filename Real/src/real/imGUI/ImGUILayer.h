#pragma once

#include "Real/Core/Layer.h"
#include "Real/Events/ApplicationEvent.h"
#include "Real/Events/KeyEvent.h"
#include "Real/Events/MouseEvent.h"

namespace Real {
	class ImGUILayer : public Layer{
	public:
		ImGUILayer();
		~ImGUILayer();

		void onAttach() override;
		void onDetach() override;

		void begin();
		void end();

		void onEvent(Event& event) override;

		void blockEvents(bool block);

		void setDarkTheme();

	private:
		bool isBlockingEvents = true;
	};
}

