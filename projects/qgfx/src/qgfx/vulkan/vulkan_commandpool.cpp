#if defined(QGFX_VULKAN)

#include "qgfx/vulkan/queuefamily.h"

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
	//QueueFamilyIndices queueFamilyIndices = findQueueFamilies
}

#endif // QGFX_VULKAN
