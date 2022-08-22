include "dependencies.lua"

workspace "Real"
	architecture "x64"
	startproject "RealEditor"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "Real/vendor/glfw"
	include "Real/vendor/glad"
	include "Real/vendor/imgui"	
	include "Real/vendor/yaml-cpp"
group ""

include "Real"
include "RealEditor"
include "Sandbox"

