#include <map>
#include <vector>
#include <iostream>
#include <set>
#include <algorithm>
#include <cstring>

#if defined(QGFX_VULKAN)

#include "qgfx/vulkan/vulkan_context_handle.h"
#include "qgfx/vulkan/vulkan_rasterizer.h"
#include "qgfx/vulkan/vulkan_pipeline.h"
#include "qgfx/vulkan/vulkan_shader.h"
#include "GLFW/glfw3.h"
#include "qgfx/qassert.h"

const std::vector<const char*> validationLayers = {
	"VK_LAYER_LUNARG_standard_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#if defined(_DEBUG)
const bool enableValidationLayers = true;
static VKAPI_ATTR VkBool32 VKAPI_CALL sDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
													VkDebugUtilsMessageTypeFlagsEXT messageType,
													const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
													void* userData)
{
	std::cerr << "Validation layer: " << callbackData->pMessage << std::endl;

	return VK_FALSE;
}
#else
const bool enableValidationLayers = false;
static VKAPI_ATTR VkBool32 VKAPI_CALL sDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
	void* userData)
{
	return VK_FALSE;
}
#endif

bool checkValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers)
	{
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
		{
			return false;
		}
	}

	return true;
}

std::vector<const char*> getRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if(enableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

int32_t rateDeviceSuitability(const VkPhysicalDevice device)
{
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;

	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	int32_t score = 0;

	if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	{
		score += 1000;
	}

	score += deviceProperties.limits.maxImageDimension2D;

	if(!deviceFeatures.geometryShader)
	{
		return 0;
	}

	return score;
}

VulkanContextHandle::VulkanContextHandle(GLFWwindow* window)
{
	mInstance = nullptr;
	mPhysicalDevice = nullptr;
	mCallback = 0;

	mWindow = window;

	_createInstance();
	_setupDebugCallback();
	_createSurface();
	_pickPhysicalDevice();
	_createLogicalDevice();
	_createSwapChain();
	_createImageViews();
	_createRenderPass();
	_createGraphicsPipeline();
}

VulkanContextHandle::~VulkanContextHandle()
{
	if (enableValidationLayers)
	{
		const auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(
			mInstance, "vkDestroyDebugUtilsMessengerEXT"));

		if (func != nullptr)
		{
			func(mInstance, mCallback, nullptr);
		}
	}

	for(auto imageView : mSwapChainImageViews)
	{
		vkDestroyImageView(mDevice, imageView, nullptr);
	}

	vkDestroySwapchainKHR(mDevice, mSwapChain, nullptr);

	vkDestroyDevice(mDevice, nullptr);

	vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
	vkDestroyInstance(mInstance, nullptr);

	mInstance = nullptr;
	mDevice = nullptr;
}

VkInstance VulkanContextHandle::getInstance() const
{
	return mInstance;
}

VkPhysicalDevice VulkanContextHandle::getPhysicalDevice() const
{
	return mPhysicalDevice;
}

VkDevice VulkanContextHandle::getLogicalDevice() const
{
	return mDevice;
}

VkSwapchainKHR VulkanContextHandle::getSwapChain() const
{
	return mSwapChain;
}

VkExtent2D VulkanContextHandle::getSwapChainExtent() const
{
	return mSwapChainExtent;
}

VkFormat VulkanContextHandle::getSwapChainFormat() const
{
	return mSwapChainImageFormat;
}

VulkanRasterizer* VulkanContextHandle::getRasterizer() const
{
	return mRasterizer;
}

void VulkanContextHandle::_createInstance()
{
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "QGFX Vulkan";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "QGFX";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_1;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto extensions = getRequiredExtensions();

	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	if (enableValidationLayers)
	{
		const bool result = checkValidationLayerSupport();
		QGFX_ASSERT_MSG(result, "Validation layers requested, but not available!");

		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	const VkResult result = vkCreateInstance(&createInfo, nullptr, &mInstance);

	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to create instance: %d\n", static_cast<int32_t>(result));
}

void VulkanContextHandle::_setupDebugCallback()
{
	if (enableValidationLayers)
	{
		VkDebugUtilsMessengerCreateInfoEXT callbackCreationInfo = {};
		callbackCreationInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		callbackCreationInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		callbackCreationInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		callbackCreationInfo.pfnUserCallback = sDebugCallback;
		callbackCreationInfo.pUserData = nullptr; // optional

		const auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(
			mInstance, "vkCreateDebugUtilsMessengerEXT"));

		if (func != nullptr)
		{
			func(mInstance, &callbackCreationInfo, nullptr, &mCallback);
		}
		else
		{
			std::cerr << "Extension not present!" << std::endl;
		}
	}
}

void VulkanContextHandle::_pickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(mInstance, &deviceCount, nullptr);

	QGFX_ASSERT_MSG(deviceCount != 0, "Failed to find GPUs with Vulkan support!");

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(mInstance, &deviceCount, devices.data());

	std::multimap<int, VkPhysicalDevice> candidates;

	for(const auto& device : devices)
	{
		int32_t score = rateDeviceSuitability(device);
		if(_isDeviceSuitable(device))
		{
			candidates.insert(std::make_pair(score, device));
		}
	}

	if(candidates.rbegin()->first > 0)
	{
		mPhysicalDevice = candidates.rbegin()->second;
	}

	QGFX_ASSERT_MSG(mPhysicalDevice != nullptr, "Failed to find a suitable GPU!");

	VkPhysicalDeviceProperties props;
	vkGetPhysicalDeviceProperties(mPhysicalDevice, &props);
	std::cout << "Running Vulkan on: " << props.deviceName << std::endl;
}

void VulkanContextHandle::_createLogicalDevice()
{
	QueueFamilyIndices indices = _findQueueFamilies(mPhysicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	float queuePriority = 1.0f;
	for(uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}


	VkPhysicalDeviceFeatures deviceFeatures = {};

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if(enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	const VkResult result = vkCreateDevice(mPhysicalDevice, &createInfo, nullptr, &mDevice);
	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to create logical device!");

	vkGetDeviceQueue(mDevice, indices.graphicsFamily.value(), 0, &mGraphicsQueue);
	vkGetDeviceQueue(mDevice, indices.presentFamily.value(), 0, &mPresentQueue);
}

void VulkanContextHandle::_createSurface()
{
	const VkResult result = glfwCreateWindowSurface(mInstance, mWindow, nullptr, &mSurface);

	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to create window surface!");
}

void VulkanContextHandle::_createSwapChain()
{
	const SwapChainSupportDetails swapChainSupport = _querySwapChainSupport(mPhysicalDevice);

	const VkSurfaceFormatKHR surfaceFormat = _chooseSwapSurfaceFormat(swapChainSupport.formats);
	const VkPresentModeKHR presentMode = _chooseSwapPresentMode(swapChainSupport.presentModes);
	const VkExtent2D extent = _chooseSwapExtent(swapChainSupport.capabilities);
	mSwapChainImageFormat = surfaceFormat.format;
	mSwapChainExtent = extent;

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = mSurface;

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = _findQueueFamilies(mPhysicalDevice);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if(indices.graphicsFamily != indices.presentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	const VkResult result = vkCreateSwapchainKHR(mDevice, &createInfo, nullptr, &mSwapChain);

	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to create SwapChain!");

	vkGetSwapchainImagesKHR(mDevice, mSwapChain, &imageCount, nullptr);
	mSwapChainImages.reserve(imageCount);
	vkGetSwapchainImagesKHR(mDevice, mSwapChain, &imageCount, mSwapChainImages.data());
}

void VulkanContextHandle::_createImageViews()
{
	mSwapChainImageViews.reserve(mSwapChainImages.size());

	for(size_t i = 0; i < mSwapChainImages.size(); i++)
	{
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = mSwapChainImages[i];

		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = mSwapChainImageFormat;

		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		const VkResult result = vkCreateImageView(mDevice, &createInfo, nullptr, &mSwapChainImageViews[i]);
		QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to create image views!");
	}
}

void VulkanContextHandle::_createRenderPass()
{

}

void VulkanContextHandle::_createGraphicsPipeline()
{
	qtl::shared_ptr<Shader> shader = qtl::make_shared<Shader>(this);
	shader->attachVertexShader("0010 0083 0010 00df 0000 0053\
		0000 0013 0030 00e3 0000 0043 0000 0053\
		0000 0041 0040 00d3 0000 0023 0000 0071\
		0000 0043 0000 0033 0050 0014 0000 0062\
		0000 0023 0000 0032 0040 00d3 0000 00d2\
		0000 00f2 0030 00e3 0000 0042 0000 0022\
		0000 00f2 0000 00e2 0050 0014 0000 0081\
		0000 00d0 0000 00c2 0000 00b2 0000 00d2\
		0000 00d1 0070 0005 0000 0010 0000 00a2\
		0000 00c2 0000 0060 0050 0015 0000 0000\
		0000 00a2 0000 00b2 0000 0060 0050 0015\
		0000 0092 0000 00a2 0000 0070 0040 00d3\
		0000 0072 0000 00c0 0000 0092 0000 0082\
		0050 0014 0000 0062 0000 0072 0000 0032\
		0040 00d3 0000 00c1 0000 0071 0030 00e3\
		0000 0031 0000 00c0 0030 00e3 0000 0050\
		0020 008f 0000 0030 0000 0000 0000 0040\
		0000 0020 0050 0063 0000 0041 0000 0060\
		0000 0033 0040 0002 0000 0030 0000 0013\
		0000 0003 0040 00b3 0000 0041 0000 0030\
		0000 0003 0040 0002 0000 00d1 0000 0030\
		0000 00e2 0040 0002 0000 0070 0000 0060\
		0000 0082 0040 0002 0000 0010 0000 0062\
		0000 0052 0040 00b3 0000 0032 0000 0010\
		0000 0052 0040 0002 0000 0000 0000 0042\
		0000 0032 0040 00b2 0000 0010 0000 0002\
		0000 0032 0040 0051 0000 0030 0000 0022\
		0000 0012 0040 00b3 0000 0002 0000 0030\
		0000 0012 0040 0002 0000 00f1 0000 00f1\
		0000 0060 0000 00d1 0000 0002 0060 00e1\
		0000 00e1 0000 0060 0000 00f1 0040 00c1\
		0000 0010 0000 00e1 0000 0080 0040 00b2\
		0000 0040 0000 0060 0000 00d1 0040 0071\
		0000 00b1 0000 00a1 0000 0091 0000 00c1\
		0000 0051 0060 00c2 0000 0081 0000 00d0\
		0000 00d0 0000 00b1 0000 0041 0060 00c2\
		0000 00d0 0000 0081 0000 00d0 0000 00a1\
		0000 0041 0060 00c2 0000 00d0 0000 00d0\
		0000 0081 0000 0091 0000 0041 0060 00c2\
		f308 0000 0000 0081 0000 0060 0040 00b2\
		0000 0060 0000 0071 0000 0061 0040 00b3\
		0000 0051 0000 0060 0000 0061 0040 0002\
		0000 0090 0000 0041 0000 0051 0040 00c1\
		0000 0030 0000 0060 0000 0041 0040 0071\
		0000 0021 0000 0011 0000 00f0 0000 0031\
		0000 00a0 0060 00c2 0000 0001 0000 00e0\
		0000 0021 0000 0070 0050 00c2 0000 0001\
		0000 0001 0000 0011 0000 0070 0050 00c2\
		f300 0000 0000 0001 0000 0060 0040 00b2\
		0000 00e0 0000 00d0 0000 00f0 0000 0070\
		0050 00c2 fb00 0000 0000 00e0 0000 0060\
		0040 00b2 0000 0000 0000 00d0 0000 0060\
		0040 00b2 0000 0060 0000 00c0 0000 00b0\
		0040 00b3 0000 00a0 0000 0060 0000 00b0\
		0040 0002 0000 0090 0000 0070 0000 00a0\
		0040 00c1 0000 0030 0000 0090 0000 0080\
		0040 00b2 0000 0000 0000 0002 0000 0080\
		0040 0051 0000 0020 0000 0060 0000 0070\
		0040 0071 0000 0002 0000 0060 0030 0061\
		0000 0020 0000 0030 0030 0012 0000 0020\
		0020 0031 0000 0000 0000 00e1 0000 0013\
		0040 0074 0000 00a2 0000 00b0 0000 0062\
		0040 0074 0000 0020 0000 0002 0030 0074\
		0000 0040 0000 00b0 0000 0030 0000 0002\
		0050 0084 0000 0030 0000 00b0 0000 0020\
		0000 0002 0050 0084 0000 0010 0000 00b0\
		0000 0010 0000 0002 0050 0084 0000 0000\
		0000 00b0 0000 0000 0000 0002 0050 0084\
		0000 0027 f6c6 f634 7616 2766 0000 0013\
		0050 0050 0000 8756 46e6 9487 5647 2756\
		65f5 c676 0000 0062 0060 0050 0000 0000\
		0000 0022 0030 0050 0056 36e6 1647 3796\
		44c6 c657 34f5 c676 0000 0030 0000 0002\
		0070 0060 0056 36e6 1647 3796 4407 96c6\
		34f5 c676 0000 0020 0000 0002 0070 0060\
		0000 0000 56a7 9635 47e6 96f6 05f5 c676\
		0000 0010 0000 0002 0070 0060 00e6 f696\
		4796 37f6 05f5 c676 0000 0000 0000 0002\
		0060 0060 0000 0000 8756 4727 5665 2756\
		05f5 c676 0000 0002 0060 0050 0000 3727\
		f6c6 f636 0000 0071 0040 0050 0000 0037\
		e6f6 9647 9637 f607 0000 00c0 0050 0050\
		0000 0000 e696 16d6 0000 0040 0040 0050\
		0000 3747 3656 a626 f6f5 2756 4616 8637\
		f556 4716 2716 0756 37f5 2425 14f5 c474\
		0090 0040 0000 102c 0000 0020 0030 0030\
		0000 0013 0000 0062 0000 0022 0000 0000\
		e696 16d6 0000 0040 0000 0000 0080 00f0\
		0000 0010 0000 0000 0030 00e0 0000 0000\
		0353 43e2 4647 37e2 c435 c474 0000 0010\
		0060 00b0 0000 0010 0020 0011 0000 0000\
		0000 0063 0080 0070 0010 0000 7032 2030");

	shader->attachFragmentShader("0010 0083 0010 00df\
		0000 0021 0000 0090 0030 00e3 0000 00e0\
		0000 0011 0000 0001 0000 00f0 0000 0021\
		0000 0070 0070 0005 0000 0020 0000 00d0\
		0000 0011 0000 0060 0050 0015 0000 0010\
		0000 00d0 0000 0001 0000 0060 0050 0015\
		0000 0000 0000 00d0 0000 00f0 0000 0060\
		0050 0015 0000 00c0 0000 00d0 0000 00a0\
		0040 00d3 0000 0050 0020 008f 0000 0030\
		0000 0000 0000 0040 0000 0020 0050 0063\
		f308 0000 0000 00e0 0000 0060 0040 00b2\
		0000 0010 0000 00c0 0000 00b0 0040 00b3\
		0000 00a0 0000 0010 0000 00b0 0040 0002\
		0000 0030 0000 0060 0000 00a0 0040 0071\
		0000 0030 0000 0090 0000 0080 0040 00b3\
		0000 0070 0000 0030 0000 0080 0040 0002\
		0000 0040 0000 0060 0000 0070 0040 0071\
		0000 0002 0000 0060 0030 0061 0000 0020\
		0000 0030 0030 0012 0000 0020 0020 0031\
		0000 0000 0000 00e1 0000 00c0 0040 0074\
		0000 0000 0000 00e1 0000 0090 0040 0074\
		0000 0027 f6c6 f634 7616 2766 0000 00c0\
		0050 0050 0000 0000 27f6 c6f6 3447 57f6\
		0000 0090 0050 0050 0000 0000 e696 16d6\
		0000 0040 0040 0050 0000 3747 3656 a626\
		f6f5 2756 4616 8637 f556 4716 2716 0756\
		37f5 2425 14f5 c474 0090 0040 0000 102c\
		0000 0020 0030 0030 0000 0070 0000 0040\
		0030 0001 0000 00c0 0000 0090 0000 0000\
		e696 16d6 0000 0040 0000 0040 0070 00f0\
		0000 0010 0000 0000 0030 00e0 0000 0000\
		0353 43e2 4647 37e2 c435 c474 0000 0010\
		0060 00b0 0000 0010 0020 0011 0000 0000\
		0000 0031 0080 0070 0010 0000 7032 2030");

	shader->compile();

	// Load shaders before this
	mRasterizer = new VulkanRasterizer(this);
	mPipeline = new VulkanPipeline(this);

	mPipeline->addShader(shader);

	mPipeline->construct();
}

VkSurfaceFormatKHR VulkanContextHandle::_chooseSwapSurfaceFormat(
	const qtl::vector<VkSurfaceFormatKHR>& availableFormats) const
{
	if(availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
	{
		return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	for(const auto& availableFormat : availableFormats)
	{
		if(availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR VulkanContextHandle::_chooseSwapPresentMode(
	const qtl::vector<VkPresentModeKHR>& availablePresentModes) const
{
	VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

	for(const auto& availablePresentMode : availablePresentModes)
	{
		if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
		else if(availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
		{
			bestMode = availablePresentMode;
		}
	}

	return bestMode;
}

VkExtent2D VulkanContextHandle::_chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const
{
	if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	else
	{
		VkExtent2D actualExtent = { 1280, 720 }; // TODO (Roderick) : Don't have this hardcoded.

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

VulkanContextHandle::QueueFamilyIndices VulkanContextHandle::_findQueueFamilies(const VkPhysicalDevice device) const
{
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int32_t i = 0;
	for(const auto& queueFamily : queueFamilies)
	{
		if(queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, mSurface, &presentSupport);

		if(queueFamily.queueCount > 0 && presentSupport)
		{
			indices.presentFamily = i;
		}

		if(indices.isComplete())
		{
			break;
		}

		i++;
	}

	return indices;
}

VulkanContextHandle::SwapChainSupportDetails VulkanContextHandle::_querySwapChainSupport(const VkPhysicalDevice device) const
{
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, mSurface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &formatCount, nullptr);

	if(formatCount != 0)
	{
		details.formats.reserve(formatCount);
		for(size_t i = 0; i < formatCount; i++)
		{
			details.formats.emplace_back();
		}
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &presentModeCount, nullptr);

	if(presentModeCount != 0)
	{
		details.presentModes.reserve(presentModeCount);
		for (size_t i = 0; i < presentModeCount; i++)
		{
			details.presentModes.emplace_back();
		}
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &presentModeCount, details.presentModes.data());
	}

	return details;
}


bool VulkanContextHandle::_isDeviceSuitable(const VkPhysicalDevice device) const
{
	QueueFamilyIndices indices = _findQueueFamilies(device);

	const bool extensionSupported = _checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if(extensionSupported)
	{
		SwapChainSupportDetails swapChainSupport = _querySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.isComplete() && extensionSupported && swapChainAdequate;
}

bool VulkanContextHandle::_checkDeviceExtensionSupport(const VkPhysicalDevice device) const
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for(const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

#endif // QGFX_VULKAN
