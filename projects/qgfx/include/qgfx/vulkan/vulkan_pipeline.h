#ifndef vulkan_pipeline_h__
#define vulkan_pipeline_h__

#include <vulkan/vulkan.h>

#include <qtl/shared_ptr.h>
#include <qtl/vector.h>

#include "qgfx/api/ipipeline.h"
#include "qgfx/context_handle.h"

class VulkanPipeline : public IPipeline
{
	public:
		explicit VulkanPipeline(ContextHandle* context);
		~VulkanPipeline();

		void setTopology(const Topology& topology) override;
		void addShader(const qtl::shared_ptr<Shader> shader) override;

	private:
		VkPipelineLayout mLayout;
		VkRenderPass mRenderPass;
		ContextHandle* mHandle;

		qtl::vector<qtl::shared_ptr<Shader>> mShaders;
};

#endif // vulkan_pipeline_h__