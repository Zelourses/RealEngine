project "Glad"
	kind "StaticLib"
	language "C"
	staticruntime "On"

	targetdir("out/"..outputDir .. "/%{prj.name}")
	objdir("out/build/"..outputDir .. "/%{prj.name}")

	files {
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
	}

	includedirs {
		"include"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
