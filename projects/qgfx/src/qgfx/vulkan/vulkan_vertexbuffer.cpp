#if defined(QGFX_VULKAN)

#include "qgfx/vulkan/vulkan_vertexbuffer.h"

#include <array>
#include "qgfx/qassert.h"
#include <cstring>

#include "qgfx/vulkan/vulkan_memory.h"

VulkanVertexBuffer::VulkanVertexBuffer(ContextHandle* handle) : IVertexBuffer(handle)
{
	mBuffer = VK_NULL_HANDLE;
	mMemory = VK_NULL_HANDLE;
	mBindingDescription = {};
	mSize = 0;
	mData = nullptr;
}

VulkanVertexBuffer::~VulkanVertexBuffer()
{
	vkDestroyBuffer(mHandle->getLogicalDevice(), mBuffer, nullptr);
	vkFreeMemory(mHandle->getLogicalDevice(), mMemory, nullptr);
}

void VulkanVertexBuffer::setData(void* data, const size_t size)
{
	mSize = size;
	mData = data;
}

void VulkanVertexBuffer::setLayout(const VertexBufferLayout& layout)
{
	mLayout = layout;

	mBindingDescription.binding = 0;
	mBindingDescription.stride = static_cast<uint32_t>(layout.getStride());
	mBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	for(size_t i = 0; i < layout.getLayout().size(); i++)
	{
		VkVertexInputAttributeDescription desc = {};
		desc.binding = 0;
		desc.location = static_cast<uint32_t>(i);
		desc.format = static_cast<VkFormat>(layout.getLayout()[i].type);
		desc.offset = static_cast<uint32_t>(layout.getLayout()[i].offset);

		mAttributeDescriptions.push_back(desc);
	}
}

bool VulkanVertexBuffer::construct()
{
	VkBufferCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	createInfo.size = mSize;
	createInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkResult result = vkCreateBuffer(mHandle->getLogicalDevice(), &createInfo, nullptr, &mBuffer);

	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to create vertex buffer");

	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(mHandle->getLogicalDevice(), mBuffer, &memoryRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memoryRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(mHandle->getPhysicalDevice(), memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	result = vkAllocateMemory(mHandle->getLogicalDevice(), &allocInfo, nullptr, &mMemory);

	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to allocate vertex buffer memory");

	vkBindBufferMemory(mHandle->getLogicalDevice(), mBuffer, mMemory, 0);

	void* data;
	vkMapMemory(mHandle->getLogicalDevice(), mMemory, 0, createInfo.size, 0, &data);
	memcpy(data, mData, static_cast<size_t>(createInfo.size));
	vkUnmapMemory(mHandle->getLogicalDevice(), mMemory);

	return result == VK_SUCCESS;
}

VertexBufferLayout& VulkanVertexBuffer::getLayout()
{
	return mLayout;
}

void VulkanVertexBuffer::bind()
{
	// TODO (Roderick): bind to pipeline list
}

void VulkanVertexBuffer::unbind()
{
	// TODO (Roderick): unbind from pipeline list
}

#endif // QGFX_VULKAN