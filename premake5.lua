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
    IncludeDir["Glad"] = "dependencies/Glad/include"
    IncludeDir["GLFW"] = "dependencies/GLFW/include"
    IncludeDir["Vulkan"] = "dependencies/Vulkan/include"
    IncludeDir["QTL"] = "dependencies/qtl/include"

    LibDir = {}
    LibDir["QTL"] = "dependencies/qtl/lib"

project "qgfx"
    location "projects/%{prj.name}"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("intermediates/" .. outputdir .. "/%{prj.name}")

    dependson { "Glad", "GLFW" }

    files
    {
        "projects/%{prj.name}/include/**.h",
        "projects/%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "projects/%{prj.name}/include",
        "%{IncludeDir.QTL}"
    }

    libdirs
    {
        "%{LibDir.QTL}"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "Off"
        systemversion "latest"

    filter "system:linux"
        buildoptions "-std=c++17"
        staticruntime "Off"
        linkoptions "-pthread"
        linkoptions "-lX11"
        linkoptions "-ldl"

        libdirs 
        {
            "/usr/lib", 
            "usr/local/lib"
        }

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
            "Glad",
        }

        defines
        {
        	"QGFX_OPENGL"
        }
        
    filter { "configurations:OpenGLDebug or OpenGLRelease or OpenGLDistribution", "system:windows" }
        links
        {
            "opengl32.lib"
        }

    filter { "configurations:OpenGLDebug or OpenGLRelease or OpenGLDistribution", "system:linux" }
        links
        {
            "GL"
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

    filter { "configurations:VulkanDebug or OpenGLDebug", "platforms:x86" }
        links
        {
            "qtlDEBUGx86"
        }
    
    filter { "configurations:VulkanDebug or OpenGLDebug", "platforms:x64" }
        links
        {
            "qtlDEBUGx64"
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

    filter { "configurations:VulkanRelease or OpenGLRelease or configurations:VulkanDistribution or OpenGLDistribution", "platforms:x86" }
        links
        {
            "qtlRELEASEx86"
        }
    
    filter { "configurations:VulkanRelease or OpenGLRelease or configurations:VulkanDistribution or OpenGLDistribution", "platforms:x64" }
        links
        {
            "qtlRELEASEx64"
        }

    filter {} 
    
project "qgfx-test"
    location "projects/%{prj.name}"
    kind "ConsoleApp"
    language "C++"

    dependson { "qgfx" }

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("intermediates/" .. outputdir .. "/%{prj.name}")

    ignoredefaultlibraries 
    {
        "LIBCMT", 
        "LIBCMTD"
    }

    files
    {
        "projects/%{prj.name}/include/**.h",
        "projects/%{prj.name}/src/**.cpp"
    }

    links
    {
        "qgfx"
    }

    includedirs
    {
        "projects/qgfx/include",
        "%{IncludeDir.QTL}"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "Off"
        systemversion "latest"

    filter "system:linux"
        buildoptions "-std=c++17"
        staticruntime "Off"

        linkoptions
        {
            "-lpthread",
            "-lX11",
            "-ldl"
        }

        libdirs 
        {
            "/usr/lib", 
            "usr/local/lib"
        }  

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
        }

        defines
        {
        	"QGFX_OPENGL"
        }
        
    filter { "configurations:OpenGLDebug or OpenGLRelease or OpenGLDistribution", "system:windows" }
        links
        {
            "opengl32.lib"
        }

    filter { "configurations:OpenGLDebug or OpenGLRelease or OpenGLDistribution", "system:linux" }
        links 
        {
            "GL"
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
    		"GLFW"
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
        
    filter { "configurations:VulkanDebug or VulkanRelease or VulkanDistribution", "system:linux" }
        links
        {
            "vulkan"
        }

    filter { "configurations:VulkanDebug or VulkanRelease or VulkanDistribution", "system:windows" }
        links 
        {
            "vulkan-1"
        }

    filter {}