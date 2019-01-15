#include "qgfx/vulkan/vulkan_vertexbuffer.h"

#include <array>
#include "qgfx/qassert.h"

ContextHandle* handle;

VulkanVertexBuffer::VulkanVertexBuffer(ContextHandle* context)
{
	handle = context;

	mBuffer = VK_NULL_HANDLE;
	mBindingDescription = {};
	mSize = 0;
}

VulkanVertexBuffer::~VulkanVertexBuffer()
{
	vkDestroyBuffer(handle->getLogicalDevice(), mBuffer, nullptr);
}

void VulkanVertexBuffer::setData(void* data, const size_t size)
{
	mSize = size;
}

void VulkanVertexBuffer::setLayout(const VertexLayout& layout)
{
	mLayout = layout;

	mBindingDescription.binding = 0;
	mBindingDescription.stride = 0; // TODO (Roderick): use layout.getStride();
	mBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	for(size_t i = 0; i < 0; i++) // TODO (Roderick): use layout.size();
	{
		VkVertexInputAttributeDescription desc = {};
		desc.binding = 0;
		desc.location = static_cast<uint32_t>(i);
		desc.format = static_cast<VkFormat>(0); // TODO (Roderick): use current layout element format
		desc.offset = 0; // TODO (Roderick): use current layout element offset

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

	const VkResult result = vkCreateBuffer(handle->getLogicalDevice(), &createInfo, nullptr, &mBuffer);

	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to create vertex buffer");

	return result == VK_SUCCESS;
}

VertexLayout& VulkanVertexBuffer::getLayout()
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