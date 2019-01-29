#ifndef vulkan_pipeline_h__
#define vulkan_pipeline_h__

#include <vulkan/vulkan.h>

#include <qtl/vector.h>

#include "qgfx/api/ipipeline.h"
#include "qgfx/context_handle.h"

class VulkanPipeline : public IPipeline
{
	public:
		explicit VulkanPipeline(ContextHandle* handle);
		~VulkanPipeline();

		void construct() override;

		void setTopology(const Topology& topology) override;
		Shader* addShader() override;

		VkRenderPass getRenderPass() const;
		VkPipeline getPipeline() const;

	private:
		VkPipelineLayout mLayout;
		VkRenderPass mRenderPass;
		VkPipeline mPipeline;

		VkPipelineInputAssemblyStateCreateInfo mInputAssembly;

		qtl::vector<Shader*> mShaders;
};

#endif // vulkan_pipeline_h__