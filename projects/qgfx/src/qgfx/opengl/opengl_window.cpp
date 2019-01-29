#if defined(QGFX_OPENGL)

#include "qgfx/qassert.h"
#include "qgfx/opengl/opengl_window.h"

OpenGLWindow::~OpenGLWindow()
{
	glfwDestroyWindow(mHandle);
}

void OpenGLWindow::construct(const WindowCreationParameters& params)
{
	construct(params.width, params.height, params.title, params.fullscreen, params.vsync);
}

void OpenGLWindow::construct(const uint32_t width, const uint32_t height, const qtl::string & title, const bool fullscreen, const bool vsync)
{
	if (!glfwInit())
	{
		QGFX_ASSERT_MSG(false, "Failed to initialize GLFW!");
		return;
	}

	mHandle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!mHandle)
	{
		QGFX_ASSERT_MSG(false, "Failed to create window!");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(mHandle);
	glfwSwapInterval(vsync ? 1 : 0);
}

void* OpenGLWindow::getPlatformHandle() const
{
	return mHandle;
}

bool OpenGLWindow::shouldClose() const
{
	return glfwWindowShouldClose(mHandle);
}

void OpenGLWindow::poll() const
{
	glfwPollEvents();
}

#endif