RealDirs = {}
RealDirs["src"] = "%{wks.location}/Real/src"
RealDirs["vendor"] = "%{wks.location}/Real/vendor"

project "Real"
	kind "StaticLib"
	language "C++"
	cppdialect "C++latest"
	staticruntime "off"

	targetdir ("%{wks.location}/out/"..outputDir.. "/%{prj.name}")
	objdir ("%{wks.location}/out/build/"..outputDir.. "/%{prj.name}")

	pchheader "repch.h"
	pchsource "src/repch.cpp"

	files {
		"src/**.h",
		"src/**.cpp",
        "%{includeDir.stb_image}/**.hpp",
        "%{includeDir.stb_image}/**.cpp",
	}

	includedirs {
		"src",
		"%{includeDir.GLFW}",
		"%{includeDir.spdlog}",
		"%{includeDir.Glad}",
		"%{includeDir.ImGUI}",
		"%{includeDir.glm}",
		"%{includeDir.stb_image}",
		"%{includeDir.entt}",
	}

	links{
		"GLFW",
		"Glad",
		"ImGUI",
		"opengl32.lib"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE",
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"RE_PLATFORM_WINDOWS",
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