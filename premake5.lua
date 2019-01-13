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
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("intermediates/" .. outputdir .. "/%{prj.name}")

    dependson { "Glad", "GLFW" }

    files
    {
        "projects/%{prj.name}/includes/**.h",
        "projects/%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "projects/%{prj.name}/includes"
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

        defines
        {
        	"QGFX_OPENGL"
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
    		"%{IncludeDir.Vulkan}",
    		"%{IncludeDir.GLFW}"
    	}

    	links
    	{
    		"GLFW",
    		"vulkan-1"
    	}

    	defines
        {
        	"QGFX_VULKAN"
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

   	filter { "configurations:VulkanDebug or OpenGLDebug" }
   		defines
   		{
   			"_DEBUG"
   		}

   	filter { "configurations:VulkanRelease or OpenGLRelease" }
   		defines
   		{
   			"_RELEASE"
   		}

   	filter { "configurations:VulkanDistribution or OpenGLDistribution" }
   		defines
   		{
   			"_DIST"
   		}

    filter {} 
    
project "qgfx-test"
    location "projects/%{prj.name}"
    kind "ConsoleApp"
    language "C++"

    dependson { "qgfx" }

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("intermediates/" .. outputdir .. "/%{prj.name}")

    ignoredefaultlibraries {"LIBCMT"}

    files
    {
        "projects/%{prj.name}/includes/**.h",
        "projects/%{prj.name}/src/**.cpp"
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

        defines
        {
        	"QGFX_OPENGL"
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
            "%{IncludeDir.Vulkan}",
            "%{IncludeDir.GLFW}"
        }

        links
    	{
    		"GLFW",
    		"vulkan-1"
    	}

    	defines
        {
        	"QGFX_VULKAN"
    	}

    filter { "configurations:VulkanDebug or VulkanRelease or VulkanDistribution", "platforms:x86" }
    	bindirs
    	{
    		"dependencies/Vulkan/bin32"
   		}

   		libdirs
    	{
    		"dependencies/Vulkan/lib32"
    	}

    filter { "configurations:VulkanDebug or VulkanRelease or VulkanDistribution", "platforms:x64" }
    	bindirs
    	{
    		"dependencies/Vulkan/bin"
   		}

   		libdirs
    	{
    		"dependencies/Vulkan/lib"
    	}

    filter {}