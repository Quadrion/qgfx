#include "qgfx/vulkan/vulkan_vertexbuffer.h"

#include <array>
#include "qgfx/qassert.h"

ContextHandle* handle;

VulkanVertexBuffer::VulkanVertexBuffer(ContextHandle* context)
{
	handle = context;

	mBuffer = VK_NULL_HANDLE;
	mMemory = VK_NULL_HANDLE;
	mBindingDescription = {};
	mSize = 0;
	mData = nullptr;
}

VulkanVertexBuffer::~VulkanVertexBuffer()
{
	vkDestroyBuffer(handle->getLogicalDevice(), mBuffer, nullptr);
	vkFreeMemory(handle->getLogicalDevice(), mMemory, nullptr);
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
	mBindingDescription.stride = layout.getStride();
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

	VkResult result = vkCreateBuffer(handle->getLogicalDevice(), &createInfo, nullptr, &mBuffer);

	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to create vertex buffer");

	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(handle->getLogicalDevice(), mBuffer, &memoryRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memoryRequirements.size;
	allocInfo.memoryTypeIndex = _findMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	result = vkAllocateMemory(handle->getLogicalDevice(), &allocInfo, nullptr, &mMemory);

	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to allocate vertex buffer memory");

	vkBindBufferMemory(handle->getLogicalDevice(), mBuffer, mMemory, 0);

	void* data;
	vkMapMemory(handle->getLogicalDevice(), mMemory, 0, createInfo.size, 0, &data);
	memcpy(data, mData, static_cast<size_t>(createInfo.size));
	vkUnmapMemory(handle->getLogicalDevice(), mMemory);

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

uint32_t VulkanVertexBuffer::_findMemoryType(const uint32_t typeFilter, const VkMemoryPropertyFlags properties) const
{
	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(handle->getPhysicalDevice(), &memoryProperties);

	for(uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
	{
		if(typeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	QGFX_ASSERT_MSG(false, "Failed to find suitable memory type");
	return 0;
}
