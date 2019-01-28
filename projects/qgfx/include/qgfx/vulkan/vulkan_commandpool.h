#ifndef vulkan_commandpool_h__
#define vulkan_commandpool_h__

#include <vulkan/vulkan.h>

#include "qgfx/api/icommandpool.h"
#include "qgfx/context_handle.h"

class VulkanCommandPool : public ICommandPool
{
	public:
		explicit VulkanCommandPool(ContextHandle* context);
		~VulkanCommandPool();

		void addCommandBuffer(const qtl::shared_ptr<CommandBuffer>& buffer) override;
		qtl::vector<qtl::shared_ptr<CommandBuffer>> getBuffers() override;
		void construct() override;

	private:
		VkCommandPool mCommandPool;
		qtl::vector<VkCommandBuffer> mVulkanBuffers;
};

#endif // vulkan_commandpool_h__