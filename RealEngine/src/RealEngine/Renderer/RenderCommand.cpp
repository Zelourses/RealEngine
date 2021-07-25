#include "repch.h"
#include "RenderCommand.h"

#include "platform/OpenGL/OpenGLRendererAPI.h"

namespace RealEngine {
	RendererAPI* RenderCommand::rendererAPI = new OpenGLRendererAPI;
}
