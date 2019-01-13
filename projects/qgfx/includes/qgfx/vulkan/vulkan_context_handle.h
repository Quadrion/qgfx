#ifndef vulkan_context_handle_h__
#define vulkan_context_handle_h__

#include <vulkan/vulkan.h>

class VulkanContextHandle
{
	public:
		VulkanContextHandle();
		~VulkanContextHandle();

		VulkanContextHandle(const VulkanContextHandle&) = delete;
		VulkanContextHandle(VulkanContextHandle&&) noexcept = delete;

		VulkanContextHandle& operator = (const VulkanContextHandle&) = delete;
		VulkanContextHandle& operator = (VulkanContextHandle&&) noexcept = delete;

		VkInstance getInstance() const;
	private:
		VkInstance mInstance;
		VkDebugUtilsMessengerEXT mCallback;
		VkPhysicalDevice mPhysicalDevice;

		void _createInstance();
		void _setupDebugCallback();
		void _pickPhysicalDevice();
};

#endif // vulkan_context_handle_h__