#ifndef vulkan_context_handle_h__
#define vulkan_context_handle_h__

#include <optional>

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
		VkPhysicalDevice getPhysicalDevice() const;
		VkDevice getLogicalDevice() const;

	private:
		VkInstance mInstance;
		VkDebugUtilsMessengerEXT mCallback;
		VkPhysicalDevice mPhysicalDevice;
		VkDevice mDevice;

		VkQueue mGraphicsQueue;

		VkSurfaceKHR mSurface;

		struct QueueFamilyIndices
		{
			std::optional<uint32_t> graphicsFamily;

			bool isComplete() const
			{
				return graphicsFamily.has_value();
			}
		};

		void _createInstance();
		void _setupDebugCallback();
		void _pickPhysicalDevice();
		void _createLogicalDevice();


		QueueFamilyIndices _findQueueFamilies(const VkPhysicalDevice device) const;
		bool _isDeviceSuitable(const VkPhysicalDevice device) const;
};

#endif // vulkan_context_handle_h__