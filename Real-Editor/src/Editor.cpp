#include <Real.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

#include "platform/OpenGL/OpenGLShader.h"
#include "EditorLayer.h"

// --- Engine entry point --------
#include "Real/Core/EntryPoint.h"

namespace Real {
	class RealEditor : public Application {
	public:
		RealEditor() {
			pushLayer(new EditorLayer());
		}
		~RealEditor() {
		}
	};

	Application* createApplication() {
		return new RealEditor();
	}
}
