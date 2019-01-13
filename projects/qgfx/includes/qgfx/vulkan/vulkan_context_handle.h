#ifndef vulkan_context_handle_h__
#define vulkan_context_handle_h__

#include <optional>
#include <vector>

#include <vulkan/vulkan.h>
#include "GLFW/glfw3.h"

class VulkanContextHandle
{
	public:
		explicit VulkanContextHandle(GLFWwindow* window);
		~VulkanContextHandle();

		VulkanContextHandle(const VulkanContextHandle&) = delete;
		VulkanContextHandle(VulkanContextHandle&&) noexcept = delete;

		VulkanContextHandle& operator = (const VulkanContextHandle&) = delete;
		VulkanContextHandle& operator = (VulkanContextHandle&&) noexcept = delete;

		VkInstance getInstance() const;
		VkPhysicalDevice getPhysicalDevice() const;
		VkDevice getLogicalDevice() const;

	private:
		GLFWwindow* mWindow;

		VkInstance mInstance;
		VkDebugUtilsMessengerEXT mCallback;
		VkPhysicalDevice mPhysicalDevice;
		VkDevice mDevice;

		VkQueue mGraphicsQueue;
		VkQueue mPresentQueue;

		VkSurfaceKHR mSurface;
		VkSwapchainKHR mSwapChain;
		VkFormat mSwapChainImageFormat;
		VkExtent2D mSwapChainExtent;

		std::vector<VkImage> mSwapChainImages;
		std::vector<VkImageView> mSwapChainImageViews;

		struct QueueFamilyIndices
		{
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;

			bool isComplete() const
			{
				return graphicsFamily.has_value() && presentFamily.has_value();
			}
		};

		struct SwapChainSupportDetails
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

		void _createInstance();
		void _setupDebugCallback();
		void _pickPhysicalDevice();
		void _createLogicalDevice();
		void _createSurface();
		void _createSwapChain();
		void _createImageViews();
		void _createGraphicsPipeline();

		VkSurfaceFormatKHR _chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
		VkPresentModeKHR _chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;
		VkExtent2D _chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;

		QueueFamilyIndices _findQueueFamilies(const VkPhysicalDevice device) const;
		SwapChainSupportDetails _querySwapChainSupport(const VkPhysicalDevice device) const;
		bool _isDeviceSuitable(const VkPhysicalDevice device) const;
		bool _checkDeviceExtensionSupport(const VkPhysicalDevice device) const;
};

#endif // vulkan_context_handle_h__