project "RealEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++latest"
	staticruntime "off"

	targetdir ("%{wks.location}/out/"..outputDir.. "/%{prj.name}")
	objdir ("%{wks.location}/out/build/"..outputDir.. "/%{prj.name}")

	files {
		"src/**.h",
		"src/**.cpp"
	}

	includedirs {
		"%{includeDir.spdlog}",
		"%{RealDirs.src}",
		"%{RealDirs.vendor}",
		"%{includeDir.glm}",
		"%{includeDir.entt}",
		"%{includeDir.ImGuizmo}",
	}

	links{
		"Real"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"RE_PLATFORM_WINDOWS",
			-- because I love c++latest in cppdialec, I get this warning that ruins my compilation
			"_SILENCE_CXX23_ALIGNED_STORAGE_DEPRECATION_WARNING",
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