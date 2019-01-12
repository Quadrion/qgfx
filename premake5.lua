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

    include "dependencies/GLFW"
    include "dependencies/Glad"

    outputdir = "%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}"

    IncludeDir = {}
    IncludeDir["Glad"] = "dependencies/Glad/includes"
    IncludeDir["GLFW"] = "dependencies/GLFW/include"
    IncludeDir["Vulkan"] = "dependencies/Vulkan/include"

project "qgfx"
    location "projects/%{prj.name}"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("intermediates/" .. outputdir .. "/%{prj.name}")

    dependson { "Glad", "GLFW" }

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

    filter "configurations:VulkanDebug"
        runtime "Debug"
        symbols "On"

    filter "configurations:VulkanRelease"
        runtime "Release"
        optimize "On"

    filter "configurations:VulkanDistribution"
        runtime "Release"
        optimize "Full"

    filter "configurations:VulkanDebug or VulkanRelease or VulkanDistribution"
    	includedirs
    	{
    		"%{IncludeDir.Vulkan}"
    	}

    	links
    	{
    		"vulkan-1"
    	}

    filter { "configurations:VulkanDebug or VulkanRelease or VulkanDistribution", "platforms:x86" }
    	libdirs
    	{
    		"dependencies/Vulkan/lib32"
    	}

    filter { "configurations:VulkanDebug or VulkanRelease or VulkanDistribution", "platforms:x64" }
    	libdirs
    	{
    		"dependencies/Vulkan/lib"
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

    filter "configurations:VulkanDebug"
        runtime "Debug"
        symbols "On"

    filter "configurations:VulkanRelease"
        runtime "Release"
        optimize "On"

    filter "configurations:VulkanDistribution"
        runtime "Release"
        optimize "Full"

    filter { "configurations:VulkanDebug or VulkanRelease or VulkanDistribution", "platforms:x86" }
    	bindirs
    	{
    		"dependencies/Vulkan/bin32"
   		}

    filter { "configurations:VulkanDebug or VulkanRelease or VulkanDistribution", "platforms:x64" }
    	bindirs
    	{
    		"dependencies/Vulkan/bin"
   		}

    filter {}