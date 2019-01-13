#include "qgfx/qgfx.h"

#include <iostream>

#include <GLFW/glfw3.h>
#if defined (QGFX_VULKAN)
#include <vulkan/vulkan.h>
#endif
#include <cassert>
#include "qgfx/qassert.h"
#include <vector>

const std::vector<const char*> validationLayers = {
	"VK_LAYER_LUNARG_standard_validation"
};

#if defined(_DEBUG)
const bool enableValidationLayers = true;
#else
const bool enableValidationLayers = false;
#endif

#if defined(QGFX_VULKAN)
bool checkValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for(const char* layerName : validationLayers)
	{
		bool layerFound = false;

		for(const auto& layerProperties : availableLayers)
		{
			if(strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if(!layerFound)
		{
			return false;
		}
	}

	return true;
}
#endif 

int main()
{
#if defined(QGFX_OPENGL)
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(1280, 720, "QGFX Window", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
#elif defined(QGFX_VULKAN)
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(1280, 720, "QGFX Window", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// init vulkan
	VkInstance instance;
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

	uint32_t glfwExtensionCount = 0;

	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;

	if(enableValidationLayers)
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


	const VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to create instance: %d\n", static_cast<int32_t>(result));

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	std::cout << "Available extensions: " << std::endl;
	for(const auto& extension : extensions)
	{
		std::cout << "\t" << extension.extensionName << std::endl;
	}

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}

	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);
	glfwTerminate();

#endif

	return 0;
}
