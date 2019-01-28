#if defined(QGFX_VULKAN)
#include "qgfx/qassert.h"
#include "qgfx/vulkan/vulkan_window.h"

VulkanWindow::VulkanWindow()
{
	mWindow = nullptr;
}

VulkanWindow::~VulkanWindow()
{
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

void VulkanWindow::construct(const WindowCreationParameters& params)
{
	construct(params.width, params.height, params.title, params.fullscreen, params.vsync);
}

void VulkanWindow::construct(const uint32_t width, const uint32_t height, const qtl::string& title, const bool fullscreen, const bool vsync)
{
	if (!glfwInit())
	{
		QGFX_ASSERT_MSG(false, "Failed to initialize GLFW!");
		return;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	/* Create a windowed mode window and its OpenGL context */
	mWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!mWindow)
	{
		QGFX_ASSERT_MSG(false, "Failed to create window!");
		glfwTerminate();
		return;
	}
}

void* VulkanWindow::getPlatformHandle() const
{
	return mWindow;
}

bool VulkanWindow::shouldClose() const
{
	return glfwWindowShouldClose(mWindow);
}

void VulkanWindow::poll() const
{
	glfwPollEvents();
}

#endif // QGFX_VULKAN
