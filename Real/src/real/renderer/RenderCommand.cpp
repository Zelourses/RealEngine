#include "repch.h"
#include "RenderCommand.h"

#include "platform/OpenGL/OpenGLRendererAPI.h"

namespace Real {
	RendererAPI* RenderCommand::rendererAPI = new OpenGLRendererAPI;
}
