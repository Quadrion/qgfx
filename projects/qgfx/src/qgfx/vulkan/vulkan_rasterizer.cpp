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

VulkanRasterizer::VulkanRasterizer(ContextHandle* context)
{
	mHandle = context;

	rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.depthClampEnable = VK_FALSE;

	setFrontFace(FrontFace::Clockwise);
	setCullMode(CullMode::Back);
	setLineWidth(1.0f);
	setPolygonMode(PolygonMode::Fill, CullMode::Front);

	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f;
	rasterizer.depthBiasClamp = 0.0f;
	rasterizer.depthBiasSlopeFactor = 0.0f;
}

VulkanRasterizer::~VulkanRasterizer()
{
	
}

void VulkanRasterizer::setCullMode(const CullMode mode)
{
	rasterizer.cullMode = qgfxCullmodeToVulkan(mode);
}

void VulkanRasterizer::setFrontFace(const FrontFace face)
{
	rasterizer.frontFace = qgfxFrontfaceToVulkan(face);
}

void VulkanRasterizer::setPolygonMode(const PolygonMode mode, const CullMode face)
{
	rasterizer.polygonMode = qgfxPolygonModeToVulkan(mode);
}

void VulkanRasterizer::setLineWidth(const float lineWidth)
{
	rasterizer.lineWidth = lineWidth;
}

#endif // QGFX_VULKAN