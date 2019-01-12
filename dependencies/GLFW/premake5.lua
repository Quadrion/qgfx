project "GLFW"
    kind "StaticLib"
    language "C"

    outputdir = "%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}"
    
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
        "include/GLFW/glfw3.h",
        "include/GLFW/glfw3native.h",
        "src/glfw_config.h",
        "src/context.c",
        "src/init.c",
        "src/input.c",
        "src/monitor.c",
        "src/vulkan.c",
        "src/window.c"
    }
    
	filter "system:windows"
        cdialect "C11"
        systemversion "10.0.17134.0"
        staticruntime "on"

        links
        {
            "gdi32"
        }
        
        files
        {
            "src/win32_init.c",
            "src/win32_joystick.c",
            "src/win32_monitor.c",
            "src/win32_time.c",
            "src/win32_window.c",
            "src/wgl_context.c",
            "src/egl_context.c"
        }

		defines 
		{ 
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
		}
		
    filter { "system:windows", "configurations:Release" }
        runtime "release"

    filter "system:linux"
        buildoptions { "-std=c11" }
        systemversion "10.0.17134.0"
        staticruntime "on"
        
        files
        {
            "src/x11_init.c",
            "src/linux_joystick.c",
            "src/x11_monitor.c",
            "src/posix_time.c",
            "src/posix_thread.c",
            "src/x11_window.c",
            "src/glx_context.c",
            "src/egl_context.c",
            "src/osmesa_context.c",
            "src/xkb_unicode.c"
        }

		defines 
		{ 
            "_GLFW_X11"
		}
