#ifndef vulkan_rasterizer_h__
#define vulkan_rasterizer_h__

#include "qgfx/api/irasterizer.h"
#include "qgfx/context_handle.h"

class VulkanRasterizer : public IRasterizer
{
	public:
		explicit VulkanRasterizer(ContextHandle* context);
		~VulkanRasterizer();

		void setCullMode(const CullMode mode) override;
		void setFrontFace(const FrontFace face) override;

		void setPolygonMode(const PolygonMode mode, const CullMode face) override;

		void setLineWidth(const float lineWidth) override;

		void setDepthTest(const bool enabled) override;

	private:
		VkPipelineRasterizationStateCreateInfo rasterizer;
};

#endif // vulkan_rasterizer_h__