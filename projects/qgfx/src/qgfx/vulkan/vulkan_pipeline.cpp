#if defined(QGFX_VULKAN)

#include "qgfx/vulkan/vulkan_pipeline.h"
#include "qgfx/qassert.h"

#include "qgfx/qgfx.h"

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

VulkanPipeline::VulkanPipeline(ContextHandle* context) : IPipeline(context)
{

}

VulkanPipeline::~VulkanPipeline()
{
	vkDestroyPipelineLayout(mHandle->getLogicalDevice(), mLayout, nullptr);
	vkDestroyRenderPass(mHandle->getLogicalDevice(), mRenderPass, nullptr);
}

void VulkanPipeline::construct()
{
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
	viewport.width = static_cast<float>(mHandle->getSwapChainExtent().width);
	viewport.height = static_cast<float>(mHandle->getSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = mHandle->getSwapChainExtent();

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineMultisampleStateCreateInfo multiSampling = {};
	multiSampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multiSampling.sampleShadingEnable = VK_FALSE;
	multiSampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multiSampling.minSampleShading = 1.0f;
	multiSampling.pSampleMask = nullptr;
	multiSampling.alphaToCoverageEnable = VK_FALSE;
	multiSampling.alphaToOneEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	VkDynamicState dynamicStates[] =
	{
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_BLEND_CONSTANTS
	};

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;

	VkResult result = vkCreatePipelineLayout(mHandle->getLogicalDevice(), &pipelineLayoutInfo, nullptr, &mLayout);
	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to create pipeline layout");

	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = mHandle->getSwapChainFormat();
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;

	result = vkCreateRenderPass(mHandle->getLogicalDevice(), &renderPassInfo, nullptr, &mRenderPass);
	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to create render pass");

	uint32_t stageCount = 0;
	qtl::vector<VkPipelineShaderStageCreateInfo> stages;

	for (const auto& shader : mShaders)
	{
		stageCount += shader->getStageCount();
		for (auto stage : shader->getStages())
		{
			VkPipelineShaderStageCreateInfo info = *reinterpret_cast<VkPipelineShaderStageCreateInfo*>(stage);
			stages.push_back(info);
		}
	}

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = stageCount;
	pipelineInfo.pStages = stages.data();

	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &mInputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &mHandle->getRasterizer()->getStateInfo();
	pipelineInfo.pMultisampleState = &multiSampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.layout = mLayout;
	pipelineInfo.renderPass = mRenderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	result = vkCreateGraphicsPipelines(mHandle->getLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &mPipeline);
	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to create graphics pipeline");
}

void VulkanPipeline::setTopology(const Topology& topology)
{
	mInputAssembly = {};
	mInputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	mInputAssembly.topology = qgfxTopologyToVulkan(topology);
	mInputAssembly.primitiveRestartEnable = VK_FALSE;
}

void VulkanPipeline::addShader(const qtl::shared_ptr<Shader>& shader)
{
	mShaders.push_back(shader);
}

VkRenderPass VulkanPipeline::getRenderPass() const
{
	return mRenderPass;
}

#endif // QGFX_VULKAN
