#include <map>
#if defined(QGFX_VULKAN)

#include "qgfx/vulkan/vulkan_context_handle.h"
#include "GLFW/glfw3.h"
#include "qgfx/qassert.h"
#include <vector>
#include <iostream>

const std::vector<const char*> validationLayers = {
	"VK_LAYER_LUNARG_standard_validation"
};

#if defined(_DEBUG)
const bool enableValidationLayers = true;
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
													VkDebugUtilsMessageTypeFlagsEXT messageType,
													const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
													void* userData)
{
	std::cerr << "Validation layer: " << callbackData->pMessage << std::endl;

	return VK_FALSE;
}
#else
const bool enableValidationLayers = false;
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

int32_t rateDeviceSuitability(VkPhysicalDevice device)
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

VulkanContextHandle::VulkanContextHandle()
{
	mInstance = nullptr;
	mPhysicalDevice = nullptr;
	mCallback = 0;

	_createInstance();
	_setupDebugCallback();
	_pickPhysicalDevice();
	_createLogicalDevice();
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

	vkDestroyDevice(mDevice, nullptr);

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
		callbackCreationInfo.pfnUserCallback = debugCallback;
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

	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
	queueCreateInfo.queueCount = 1;
	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures deviceFeatures = {};
	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = 0;

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

		if(indices.isComplete())
		{
			break;
		}

		i++;
	}

	return indices;
}

bool VulkanContextHandle::_isDeviceSuitable(const VkPhysicalDevice device) const
{
	QueueFamilyIndices indices = _findQueueFamilies(device);

	return indices.isComplete();
}

#endif // QGFX_VULKAN
