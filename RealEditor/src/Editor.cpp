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
		RealEditor()
			: Application({"Real Editor"}) {
			pushLayer(new EditorLayer());
		}
		~RealEditor() = default;
	};

	Application* createApplication() {
		return new RealEditor();
	}
}
