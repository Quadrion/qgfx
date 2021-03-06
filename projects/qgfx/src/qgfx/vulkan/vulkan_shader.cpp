#if defined(QGFX_VULKAN)
#include "qgfx/qassert.h"

#include "qgfx/vulkan/vulkan_shader.h"

VulkanShader::VulkanShader(ContextHandle* handle) : IShader(handle)
{
	mVertexModule = VK_NULL_HANDLE;
	mFragmentModule = VK_NULL_HANDLE;
	mGeometryModule = VK_NULL_HANDLE;
	mTesselationControlModule = VK_NULL_HANDLE;
	mTesselationEvaluationModule = VK_NULL_HANDLE;
}

VulkanShader::~VulkanShader()
{
	
}

bool VulkanShader::attachVertexShader(const qtl::vector<char>& source)
{
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = source.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(source.data());

	const VkResult result = vkCreateShaderModule(mHandle->getLogicalDevice(), &createInfo, nullptr, &mVertexModule);
	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to create vertex shader module!");

	return result == VK_SUCCESS;
}

bool VulkanShader::attachFragmentShader(const qtl::vector<char>& source)
{
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = source.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(source.data());

	const VkResult result = vkCreateShaderModule(mHandle->getLogicalDevice(), &createInfo, nullptr, &mFragmentModule);
	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to create fragment shader module!");

	return result == VK_SUCCESS;
}

bool VulkanShader::attachGeometryShader(const qtl::vector<char>& source)
{
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = source.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(source.data());

	const VkResult result = vkCreateShaderModule(mHandle->getLogicalDevice(), &createInfo, nullptr, &mGeometryModule);
	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to create geometry shader module!");

	return result == VK_SUCCESS;
}

bool VulkanShader::attachTesselationControlShader(const qtl::vector<char>& source)
{
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = source.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(source.data());

	const VkResult result = vkCreateShaderModule(mHandle->getLogicalDevice(), &createInfo, nullptr, &mTesselationControlModule);
	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to create tesselation control shader module!");

	return result == VK_SUCCESS;
}

bool VulkanShader::attachTesselationEvaluationShader(const qtl::vector<char>& source)
{
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = source.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(source.data());

	const VkResult result = vkCreateShaderModule(mHandle->getLogicalDevice(), &createInfo, nullptr, &mTesselationEvaluationModule);
	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to create tesselation evaluation shader module!");

	return result == VK_SUCCESS;
}

bool VulkanShader::compile()
{
	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = mVertexModule;
	vertShaderStageInfo.pName = "main";
	mShaderStages.push_back(vertShaderStageInfo);

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = mFragmentModule;
	fragShaderStageInfo.pName = "main";
	mShaderStages.push_back(fragShaderStageInfo);

	if(mGeometryModule != VK_NULL_HANDLE)
	{
		VkPipelineShaderStageCreateInfo geomShaderStageInfo = {};
		geomShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		geomShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		geomShaderStageInfo.module = mFragmentModule;
		geomShaderStageInfo.pName = "main";
		mShaderStages.push_back(geomShaderStageInfo);
	}

	if(mTesselationControlModule != VK_NULL_HANDLE)
	{
		VkPipelineShaderStageCreateInfo tessControlShaderStageInfo = {};
		tessControlShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		tessControlShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		tessControlShaderStageInfo.module = mFragmentModule;
		tessControlShaderStageInfo.pName = "main";
		mShaderStages.push_back(tessControlShaderStageInfo);
	}

	if(mTesselationEvaluationModule != VK_NULL_HANDLE)
	{
		VkPipelineShaderStageCreateInfo tessEvaluationShaderStageInfo = {};
		tessEvaluationShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		tessEvaluationShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		tessEvaluationShaderStageInfo.module = mFragmentModule;
		tessEvaluationShaderStageInfo.pName = "main";
		mShaderStages.push_back(tessEvaluationShaderStageInfo);
	}

	for(auto stage : mShaderStages)
	{
		QGFX_ASSERT_MSG(stage.module != VK_NULL_HANDLE, "Shader stage not valid");
	}

	return true;
}

bool VulkanShader::cleanup()
{
	vkDestroyShaderModule(mHandle->getLogicalDevice(), mVertexModule, nullptr);
	vkDestroyShaderModule(mHandle->getLogicalDevice(), mFragmentModule, nullptr);
	vkDestroyShaderModule(mHandle->getLogicalDevice(), mGeometryModule, nullptr);
	vkDestroyShaderModule(mHandle->getLogicalDevice(), mTesselationControlModule, nullptr);
	vkDestroyShaderModule(mHandle->getLogicalDevice(), mTesselationEvaluationModule, nullptr);

	return true;
}

bool VulkanShader::bind()
{
	return true;
}

bool VulkanShader::unbind()
{
	return true;
}

uint32_t VulkanShader::getStageCount() const
{
	return static_cast<uint32_t>(mShaderStages.size());
}

qtl::vector<void*> VulkanShader::getStages() const
{
	qtl::vector<void*> stages;
	for(size_t i = 0; i < mShaderStages.size(); i++)
	{
		const auto data = const_cast<VkPipelineShaderStageCreateInfo*>(mShaderStages.data());
		stages.push_back(data + i);
	}
	
	return stages;
}
#endif // QGFX_VULKAN
