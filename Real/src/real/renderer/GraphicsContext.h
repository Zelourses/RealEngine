#pragma once

namespace Real {
	class GraphicsContext {
	public:
		virtual void init()        = 0;
		virtual void swapBuffers() = 0;

		virtual ~GraphicsContext() = default;
	};
}
