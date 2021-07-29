#pragma once
//OUTER INTERFACE
//Only for RealEngine applications

#include "RealEngine/Application.h"

namespace re = RealEngine; // For easier typing

#include "RealEngine/Layer.h"
#include "RealEngine/Log.h"

#include "RealEngine/Core/Timestep.h"

#include "RealEngine/Input.h"
#include "RealEngine/KeyCodes.h"
#include "RealEngine/MouseButtonCodes.h"

#include "RealEngine/ImGUI/ImGUILayer.h"

// --- Renderer ------------------------------
#include "RealEngine/Renderer/Renderer.h"
#include "RealEngine/Renderer/RenderCommand.h"

#include "RealEngine/Renderer/Buffer.h"
#include "RealEngine/Renderer/Shader.h"
#include "RealEngine/Renderer/VertexArray.h"

#include "RealEngine/Renderer/OrthographicCamera.h"

// --- Engine entry point ------------------------------
#include "RealEngine/EntryPoint.h"
