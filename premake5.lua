workspace "RealEngine"
    architecture "x64"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includeDir = {}
includeDir["GLFW"] = "RealEngine/vendor/glfw/include"

include "RealEngine/vendor/glfw"

project "RealEngine"
    location "RealEngine"
    kind "SharedLib"
    language "C++"

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
        "%{includeDir.GLFW}"
    }

    links{
        "GLFW",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "RE_PLATFORM_WINDOWS",
            "RE_BUILD_DLL"
        }
        postbuildcommands {
            ("{COPY} %{cfg.buildtarget.relpath} ../out/" .. outputDir .. "/Sandbox")
        }
    filter "configurations:Debug"
        defines {
            "RE_DEBUG",
            "RE_ENABLE_ASSERTS"
        }
        buildoptions "/MDd"
        symbols "On"
    
    filter "configurations:Release"
        defines {
            "RE_RELEASE"
        }
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines {
            "RE_DIST"
        }
        buildoptions "/MD"
        optimize "On"


project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("out/"..outputDir.. "/%{prj.name}")
    objdir ("out/build/"..outputDir.. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "RealEngine/vendor/spdlog/include",
        "RealEngine/src"
    }

    links{
        "RealEngine"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "RE_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines {
            "RE_DEBUG"
        }
        buildoptions "/MDd"
        symbols "On"
    
    filter "configurations:Release"
        defines {
            "RE_RELEASE"
        }
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines {
            "RE_DIST"
        }
        buildoptions "/MD"
        optimize "On"
