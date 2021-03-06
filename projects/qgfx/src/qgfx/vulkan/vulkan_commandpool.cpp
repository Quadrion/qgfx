#if defined(QGFX_VULKAN)
#include "qgfx/qassert.h"

#include "qgfx/vulkan/queue_family.h"

#include "qgfx/typedefs.h"
#include "qgfx/vulkan/vulkan_commandpool.h"
#include "qgfx/vulkan/vulkan_commandbuffer.h"

VulkanCommandPool::VulkanCommandPool(ContextHandle* handle) : ICommandPool(handle)
{
	mCommandPool = nullptr;
}

VulkanCommandPool::~VulkanCommandPool()
{
	for(size_t i = 0; i < mBuffers.size(); i++)
	{
		delete mBuffers[i];
	}

	vkDestroyCommandPool(mHandle->getLogicalDevice(), mCommandPool, nullptr);
}

CommandBuffer* VulkanCommandPool::addCommandBuffer()
{
	CommandBuffer* buffer = new CommandBuffer(mHandle);
	mBuffers.push_back(buffer);

	return buffer;
}

qtl::vector<CommandBuffer*> VulkanCommandPool::getBuffers()
{
	return mBuffers;
}

void VulkanCommandPool::construct()
{
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(mHandle->getPhysicalDevice(), mHandle->getSurface());

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
	poolInfo.flags = 0;

	VkResult result = vkCreateCommandPool(mHandle->getLogicalDevice(), &poolInfo, nullptr, &mCommandPool);
	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to create command pool!");

	const size_t amount = mBuffers.size();

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = mCommandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(amount);
	mVulkanBuffers.resize(amount);

	result = vkAllocateCommandBuffers(mHandle->getLogicalDevice(), &allocInfo, mVulkanBuffers.data());
	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to create command buffers");

	for(size_t i = 0; i < mBuffers.size(); i++)
	{
		mBuffers[i]->mBuffer = mVulkanBuffers[i];
	}
}

#endif // QGFX_VULKAN
