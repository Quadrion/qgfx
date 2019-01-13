#if defined(QGFX_VULKAN)
#include "qgfx/qassert.h"

#include "qgfx/vulkan/vulkan_shader.h"

ContextHandle* handle;

VulkanShader::VulkanShader(ContextHandle* context, const std::string& aSource)
{
	handle = context;

	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = aSource.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(aSource.data());

	const VkResult result = vkCreateShaderModule(context->getLogicalDevice(), &createInfo, nullptr, &mModule);
	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to create shader module!");
}

VulkanShader::~VulkanShader()
{
	vkDestroyShaderModule(handle->getLogicalDevice(), mModule, nullptr);
}

#endif // QGFX_VULKAN