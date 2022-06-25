workspace "Real"
	architecture "x64"
	startproject "RealEditor"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includeDir = {}
includeDir["GLFW"] = "%{wks.location}/Real/vendor/glfw/include"
includeDir["Glad"] = "%{wks.location}/Real/vendor/glad/include"
includeDir["ImGUI"] = "%{wks.location}/Real/vendor/imgui"
includeDir["glm"] = "%{wks.location}/Real/vendor/glm"
includeDir["stb_image"] = "%{wks.location}/Real/vendor/stb_image"
includeDir["entt"] = "%{wks.location}/Real/vendor/entt/single_include"
includeDir["spdlog"] = "%{wks.location}/Real/vendor/spdlog/include"
includeDir["yamlcpp"] = "%{wks.location}/Real/vendor/yaml-cpp/include"


group "Dependencies"
	include "Real/vendor/glfw"
	include "Real/vendor/glad"
	include "Real/vendor/imgui"	
	include "Real/vendor/yaml-cpp"
group ""

include "Real"
include "RealEditor"
include "Sandbox"

