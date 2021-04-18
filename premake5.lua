workspace "RealEngine"
    architecture "x64"
	startproject "Sandbox"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includeDir = {}
includeDir["GLFW"] = "RealEngine/vendor/glfw/include"
includeDir["Glad"] = "RealEngine/vendor/glad/include"
includeDir["ImGUI"] = "RealEngine/vendor/imgui"
includeDir["glm"] = "RealEngine/vendor/glm"

group "Dependencies"
	include "RealEngine/vendor/glfw"
	include "RealEngine/vendor/glad"
	include "RealEngine/vendor/imgui"
	
group ""

project "RealEngine"
    location "RealEngine"
    kind "SharedLib"
    language "C++"
    staticruntime "off"

    targetdir ("out/"..outputDir.. "/%{prj.name}")
    objdir ("out/build/"..outputDir.. "/%{prj.name}")

    pchheader "repch.h"
    pchsource "RealEngine/src/repch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{includeDir.GLFW}",
        "%{includeDir.Glad}",
        "%{includeDir.ImGUI}",
        "%{includeDir.glm}"
    }

    links{
        "GLFW",
        "Glad",
        "ImGUI",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines {
            "RE_PLATFORM_WINDOWS",
            "RE_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }
        postbuildcommands {
            ("{COPY} %{cfg.buildtarget.relpath} \"../out/" ..outputDir.. "/Sandbox/\"")
        }
    filter "configurations:Debug"
        defines {
            "RE_DEBUG",
            "RE_ENABLE_ASSERTS"
        }
        runtime "Debug"
        symbols "On"
    
    filter "configurations:Release"
        defines {
            "RE_RELEASE"
        }
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines {
            "RE_DIST"
        }
        runtime "Release"
        optimize "On"


project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    staticruntime "off"

    targetdir ("out/"..outputDir.. "/%{prj.name}")
    objdir ("out/build/"..outputDir.. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "RealEngine/vendor/spdlog/include",
        "RealEngine/src",
        "%{includeDir.glm}"
    }

    links{
        "RealEngine"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines {
            "RE_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines {
            "RE_DEBUG"
        }
        runtime "Debug"
        symbols "On"
    
    filter "configurations:Release"
        defines {
            "RE_RELEASE"
        }
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines {
            "RE_DIST"
        }
        runtime "Release"
        optimize "On"
