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
includeDir["GLFW"] = "Real/vendor/glfw/include"
includeDir["Glad"] = "Real/vendor/glad/include"
includeDir["ImGUI"] = "Real/vendor/imgui"
includeDir["glm"] = "Real/vendor/glm"
includeDir["stb_image"] = "Real/vendor/stb_image"
includeDir["entt"] = "Real/vendor/entt"

group "Dependencies"
	include "Real/vendor/glfw"
	include "Real/vendor/glad"
	include "Real/vendor/imgui"	
group ""

project "Real"
	location "Real"
	kind "StaticLib"
	language "C++"
	cppdialect "C++latest"
	staticruntime "on"

	targetdir ("out/"..outputDir.. "/%{prj.name}")
	objdir ("out/build/"..outputDir.. "/%{prj.name}")

	pchheader "repch.h"
	pchsource "Real/src/repch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",

		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{includeDir.GLFW}",
		"%{includeDir.Glad}",
		"%{includeDir.ImGUI}",
		"%{includeDir.glm}",
		"%{includeDir.stb_image}",
		"%{includeDir.entt}/single_include",
	}

	links{
		"GLFW",
		"Glad",
		"ImGUI",
		"opengl32.lib"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"RE_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE",
			-- turn off all non-essential windows things for faster pch compiling
			"WIN32_LEAN_AND_MEAN",
			"VC_EXTRALEAN"
		}

	filter "configurations:Debug"
		defines {
			"RE_DEBUG",
			"RE_PROFILE"
		}
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines {
			"RE_RELEASE"
		}
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines {
			"RE_DIST"
		}
		runtime "Release"
		optimize "on"

project "RealEditor"
	location "RealEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++latest"
	staticruntime "on"

	targetdir ("out/"..outputDir.. "/%{prj.name}")
	objdir ("out/build/"..outputDir.. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Real/vendor/spdlog/include",
		"Real/src",
		"Real/vendor",
		"%{includeDir.glm}",
		"Real/vendor/entt/single_include",
	}

	links{
		"Real"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"RE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines {
			"RE_DEBUG",
			"RE_PROFILE"
		}
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines {
			"RE_RELEASE"
		}
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines {
			"RE_DIST"
		}
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++latest"
	staticruntime "on"

	targetdir ("out/"..outputDir.. "/%{prj.name}")
	objdir ("out/build/"..outputDir.. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Real/vendor/spdlog/include",
		"Real/src",
		"Real/vendor",
		"%{includeDir.glm}",
		"Real/vendor/entt/single_include",
	}

	links{
		"Real"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"RE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines {
			"RE_DEBUG",
			"RE_PROFILE"
		}
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines {
			"RE_RELEASE"
		}
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines {
			"RE_DIST"
		}
		runtime "Release"
		optimize "on"
