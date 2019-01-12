workspace "qgfx"
    configurations
    {
        "OpenGLDebug",
        "OpenGLRelease",
        "OpenGLDistribution",
        "VulkanDebug",
        "VulkanRelease",
        "VulkanDistribution",
    }

    platforms
    {
        "x86",
        "x64"
    }
    
    startproject "qgfx-test|x64"

    filter "platforms:x86"
        architecture "x86"
    
    filter "platforms:x64"
        architecture "x86_64"
    
    filter {}

    outputdir = "%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}"

    IncludeDir = {}
    IncludeDir["Glad"] = "dependencies/Glad/includes"
    IncludeDir["GLFW"] = "dependencies/GLFW/include"

project "qgfx"
    location "projects/%{prj.name}"
    kind "SharedLib"
    language "C++"

    dependson {"Glad", "GLFW"}

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("intermediates/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/includes/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/includes/**.h"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

    filter "system:linux"
        buildoptions "-std=c++17"
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

    filter "configurations:OpenGLDebug or OpenGLRelease or OpenGLDistribution"
    	include "dependencies/GLFW"
        include "dependencies/Glad"
        includedirs
        {
            "%{IncludeDir.Glad}",
            "%{IncludeDir.GLFW}"
        }
        links
        {
        	"GLFW",
            "Glad",
            "opengl32.lib"
        }

    filter {} 
    
project "qgfx-test"
    location "projects/%{prj.name}"
    kind "ConsoleApp"
    language "C++"

    dependson { "qgfx" }

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("intermediates/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/includes/**.h",
        "%{prj.name}/src/**.cpp"
    }

    links
    {
        "qgfx"
    }

    includedirs
    {
        "projects/qgfx/includes"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

    filter "system:linux"
        buildoptions "-std=c++17"
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

    filter {}