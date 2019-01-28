#ifndef vulkan_window_h__
#define vulkan_window_h__

#include "qgfx/api/iwindow.h"

class VulkanWindow : public IWindow
{
	public:
		VulkanWindow();
		~VulkanWindow();

		void construct(const WindowCreationParameters& params) override;
		void construct(const uint32_t width, const uint32_t height, const qtl::string& title, const bool fullscreen = false, const bool vsync = false) override;

		void* getPlatformHandle() const override;
	private:
		GLFWwindow* mWindow;
};

#endif // vulkan_window_h__