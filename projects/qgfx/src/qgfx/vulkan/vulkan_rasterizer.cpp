#if defined(QGFX_VULKAN)

#include "qgfx/vulkan/vulkan_rasterizer.h"

VkCullModeFlags qgfxCullmodeToVulkan(const CullMode mode)
{
	switch (mode)
	{
		case CullMode::Front: return VK_CULL_MODE_FRONT_BIT;
		case CullMode::Back: return VK_CULL_MODE_BACK_BIT;
		case CullMode::FrontAndBack: return VK_CULL_MODE_FRONT_AND_BACK;
		default: return static_cast<VkCullModeFlags>(-1);
	}
}

VkFrontFace qgfxFrontfaceToVulkan(const FrontFace face)
{
	switch (face)
	{
		case FrontFace::Clockwise: return VK_FRONT_FACE_CLOCKWISE;
		case FrontFace::CounterClockwise: return VK_FRONT_FACE_COUNTER_CLOCKWISE;
		default: return static_cast<VkFrontFace>(-1);
	}
}

VkPolygonMode qgfxPolygonModeToVulkan(const PolygonMode mode)
{
	switch (mode)
	{
		case PolygonMode::Point: return VK_POLYGON_MODE_POINT;
		case PolygonMode::Line: return VK_POLYGON_MODE_LINE;
		case PolygonMode::Fill: return VK_POLYGON_MODE_FILL;
		default: return static_cast<VkPolygonMode>(-1);
	}
}

VulkanRasterizer::VulkanRasterizer(ContextHandle* context) : IRasterizer(context)
{
	mRasterizer = {};
	mRasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	mRasterizer.rasterizerDiscardEnable = VK_FALSE;
	mRasterizer.depthClampEnable = VK_FALSE;

	setFrontFace(FrontFace::Clockwise);
	setCullMode(CullMode::Back);
	setLineWidth(1.0f);
	setPolygonMode(PolygonMode::Fill, CullMode::Front);

	mRasterizer.depthBiasEnable = VK_FALSE;
	mRasterizer.depthBiasConstantFactor = 0.0f;
	mRasterizer.depthBiasClamp = 0.0f;
	mRasterizer.depthBiasSlopeFactor = 0.0f;
}

VulkanRasterizer::~VulkanRasterizer()
{
	
}

void VulkanRasterizer::setCullMode(const CullMode mode)
{
	mRasterizer.cullMode = qgfxCullmodeToVulkan(mode);
}

void VulkanRasterizer::setFrontFace(const FrontFace face)
{
	mRasterizer.frontFace = qgfxFrontfaceToVulkan(face);
}

void VulkanRasterizer::setPolygonMode(const PolygonMode mode, const CullMode face)
{
	mRasterizer.polygonMode = qgfxPolygonModeToVulkan(mode);
}

void VulkanRasterizer::setLineWidth(const float lineWidth)
{
	mRasterizer.lineWidth = lineWidth;
}

void VulkanRasterizer::setDepthTest(const bool enabled)
{

}

VkPipelineRasterizationStateCreateInfo VulkanRasterizer::getStateInfo() const
{
	return mRasterizer;
}

#endif // QGFX_VULKAN
