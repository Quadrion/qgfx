#if defined(QGFX_VULKAN)

#include "qgfx/typedefs.h"
#include "qgfx/vulkan/vulkan_commandpool.h"

VulkanCommandPool::VulkanCommandPool(ContextHandle* context) : ICommandPool(context)
{

}

VulkanCommandPool::~VulkanCommandPool()
{
	
}

void VulkanCommandPool::addCommandBuffer(const qtl::shared_ptr<CommandBuffer>& buffer)
{
	mBuffers.push_back(buffer);
}

void VulkanCommandPool::construct()
{
	
}

#endif // QGFX_VULKAN
