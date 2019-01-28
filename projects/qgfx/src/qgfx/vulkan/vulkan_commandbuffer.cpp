#include "qgfx/qassert.h"
#if defined(QGFX_VULKAN)

#include "qgfx/vulkan/vulkan_commandbuffer.h"
#include "qgfx/vulkan/vulkan_commandpool.h"

VulkanCommandBuffer::VulkanCommandBuffer(ContextHandle* context) : ICommandBuffer(context)
{

}

VulkanCommandBuffer::~VulkanCommandBuffer()
{

}

void VulkanCommandBuffer::record()
{
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	beginInfo.pInheritanceInfo = nullptr;

	const VkResult result = vkBeginCommandBuffer(mBuffer, &beginInfo);
	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to begin recording command buffer!");
}

void VulkanCommandBuffer::end()
{
	const VkResult result = vkEndCommandBuffer(mBuffer);
	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to end recording command buffer!");
}

VkCommandBuffer VulkanCommandBuffer::getBuffer()
{
	return mBuffer;
}


#endif // QGFX_VULKAN
