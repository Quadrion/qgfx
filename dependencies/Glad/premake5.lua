project "Glad"
    kind "StaticLib"
    language "C"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("intermediates/" .. outputdir .. "/%{prj.name}")

    files
    {
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
    }

    includedirs
    {
        "include"
    }

    filter "system:windows"
        systemversion "latest"
        staticruntime "On"
        cdialect "C11"

    filter "system:linux"
        buildoptions "-std=c11"
        staticruntime "On"

    filter "configurations:OpenGLDebug"
        runtime "Debug"
        symbols "On"

    filter "configurations:OpenGLRelease"
        runtime "Release"
        optimize "On"

    filter "configurations:OpenGLDistribution"
        runtime "Release"
        optimize "Full"