#include "qgfx/qassert.h"
#if defined(QGFX_VULKAN)

#include "qgfx/vulkan/vulkan_commandbuffer.h"
#include "qgfx/vulkan/vulkan_commandpool.h"

VulkanCommandBuffer::VulkanCommandBuffer(ContextHandle* context, CommandPool* pool) : ICommandBuffer(context, pool)
{

}

VulkanCommandBuffer::~VulkanCommandBuffer()
{

}

void VulkanCommandBuffer::record()
{

}

void VulkanCommandBuffer::end()
{
	
}


#endif // QGFX_VULKAN
