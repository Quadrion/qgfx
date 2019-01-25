#ifndef vulkan_pipeline_h__
#define vulkan_pipeline_h__

#include <vulkan/vulkan.h>

#include "qgfx/api/ipipeline.h"
#include "qgfx/context_handle.h"

class VulkanPipeline : public IPipeline
{
	public:
		explicit VulkanPipeline(ContextHandle* context);
		~VulkanPipeline();

		void setTopology(const Topology& topology) override;

	private:
		VkPipelineLayout mLayout;
		VkRenderPass mRenderPass;
};

#endif // vulkan_pipeline_h__