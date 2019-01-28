#ifndef vulkan_context_handle_h__
#define vulkan_context_handle_h__

#include <optional>
#include <qtl/vector.h>

#include <vulkan/vulkan.h>
#include "GLFW/glfw3.h"

#include "qgfx/api/icontexthandle.h"

class VulkanRasterizer;
class VulkanPipeline;
/// <summary>
/// Represents an Vulkan Context Handle. Contains all the initialization objects
/// that Vulkan needs to bind to a window and render.
/// </summary>
class VulkanContextHandle : public IContextHandle
{
	public:
		/// <summary>
		/// Constructs a new Vulkan Context Handle.
		/// </summary>
		explicit VulkanContextHandle(GLFWwindow* window);

		/// <summary>
		/// Default destructor
		/// </summary>
		~VulkanContextHandle();

		VulkanContextHandle(const VulkanContextHandle&) = delete;
		VulkanContextHandle(VulkanContextHandle&&) noexcept;

		VulkanContextHandle& operator = (const VulkanContextHandle&) = delete;
		VulkanContextHandle& operator = (VulkanContextHandle&&) noexcept;

		VulkanPipeline* getPipeline() const override;
		VulkanRasterizer* getRasterizer() const override;

		void initializeGraphics() override;
		void finalizeGraphics() override;

		VulkanCommandPool* addCommandPool() override;

		void startFrame() override;
		void endFrame() override;

		void swap() override;

		/// <summary>
		/// Returns the current Vulkan Instance
		/// </summary>
		/// <returns>
		/// Returns the current VkInstance
		/// </returns>
		VkInstance getInstance() const;

		/// <summary>
		/// Returns the current Vulkan Physical Device
		/// </summary>
		/// <returns>
		/// Returns the current VkPhysicalDevice
		/// </returns>
		VkPhysicalDevice getPhysicalDevice() const;

		/// <summary>
		/// Returns the current Vulkan Device
		/// </summary>
		/// <returns>
		/// Returns the current VkDevice
		/// </returns>
		VkDevice getLogicalDevice() const;

		VkSwapchainKHR getSwapChain() const;
		VkExtent2D getSwapChainExtent() const;
		VkFormat getSwapChainFormat() const;

		qtl::vector<VkFramebuffer> getSwapChainFramebuffers() const;

		qtl::vector<VkSemaphore> getImageSemaphore() const;
		qtl::vector<VkSemaphore> getRenderSemaphore() const;
		qtl::vector<VkFence> getFences() const;

		VkQueue getGraphicsQueue() const;
		VkQueue getPresentQueue() const;

		VkSurfaceKHR getSurface() const;
	private:
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

		qtl::vector<VkImage> mSwapChainImages;
		qtl::vector<VkImageView> mSwapChainImageViews;

		VulkanRasterizer* mRasterizer;
		VulkanPipeline* mPipeline;

		qtl::vector<VkSemaphore> mImageAvailableSemaphore;
		qtl::vector<VkSemaphore> mRenderFinishedSemaphore;
		qtl::vector<VkFence> mInFlightFences;

		qtl::vector<VkFramebuffer> mSwapChainFrameBuffers;

		uint32_t mCurrentFrame;
		uint32_t mImageIndex;

		qtl::vector<VulkanCommandPool*> mCommandPools;

		void _createInstance();
		void _setupDebugCallback();
		void _pickPhysicalDevice();
		void _createLogicalDevice();
		void _createSurface();
		void _createSwapChain();
		void _createImageViews();

		void _createRenderPass();
		void _createGraphicsPipeline();

		void _createFrameBuffers();
		void _createSyncObjects();

		VkSurfaceFormatKHR _chooseSwapSurfaceFormat(const qtl::vector<VkSurfaceFormatKHR>& availableFormats) const;
		VkPresentModeKHR _chooseSwapPresentMode(const qtl::vector<VkPresentModeKHR>& availablePresentModes) const;
		VkExtent2D _chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;

		bool _isDeviceSuitable(const VkPhysicalDevice device) const;
		bool _checkDeviceExtensionSupport(const VkPhysicalDevice device) const;
};

#endif // vulkan_context_handle_h__