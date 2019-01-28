#ifndef iwindow_h__
#define iwindow_h__

#include <qtl/string.h>

#include <stdint.h>

#include "GLFW/glfw3.h"

struct WindowCreationParameters
{
	qtl::string title;

	uint32_t width;
	uint32_t height;

	bool fullscreen;
	bool vsync;
};

class IWindow
{
	public:
		IWindow() = default;
		virtual ~IWindow() = default;

		virtual void construct(const WindowCreationParameters& params) = 0;
		virtual void construct(const uint32_t width, const uint32_t height, const qtl::string& title, const bool fullscreen = false, const bool vsync = false) = 0;

		virtual void* getPlatformHandle() const = 0;
};

#endif // iwindow_h__