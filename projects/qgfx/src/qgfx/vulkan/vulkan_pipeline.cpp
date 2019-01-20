#if defined(QGFX_VULKAN)

#include "qgfx/vulkan/vulkan_pipeline.h"
#include "qgfx/qassert.h"

ContextHandle* handle;

VkPrimitiveTopology qgfxTopologyToVulkan(const Topology& topology)
{
	switch (topology)
	{
		case Topology::TriangleList: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		case Topology::TriangleStrip: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		case Topology::Line: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
		case Topology::Points: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
		default: return static_cast<VkPrimitiveTopology>(-1);
	}
}

VulkanPipeline::VulkanPipeline(ContextHandle* context)
{
	handle = context;

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.pVertexBindingDescriptions = nullptr;
	vertexInputInfo.vertexAttributeDescriptionCount = 0;
	vertexInputInfo.pVertexAttributeDescriptions = nullptr;

	setTopology(Topology::TriangleList);

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = handle->getSwapChainExtent().width;
	viewport.height = handle->getSwapChainExtent().height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = handle->getSwapChainExtent();

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;

	const VkResult result = vkCreatePipelineLayout(handle->getLogicalDevice(), &pipelineLayoutInfo, nullptr, &mLayout);
	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to create pipeline layout");
}

VulkanPipeline::~VulkanPipeline()
{
	vkDestroyPipelineLayout(handle->getLogicalDevice(), mLayout, nullptr);
}

void VulkanPipeline::setTopology(const Topology& topology)
{
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = qgfxTopologyToVulkan(topology);
	inputAssembly.primitiveRestartEnable = VK_FALSE;
}

#endif // QGFX_VULKAN