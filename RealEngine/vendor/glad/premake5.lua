project "Glad"
    kind "StaticLib"
    language "C"

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
        staticruntime "On"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter {"system:windows", "configurations:Release"}
        buildoptions "/MT"
        runtime "Release"
        optimize "on"
