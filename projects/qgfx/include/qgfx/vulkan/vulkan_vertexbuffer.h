#ifndef vulkan_vertexbuffer_h__
#define vulkan_vertexbuffer_h__

#include <vulkan/vulkan.h>

#include <qtl/vector.h>

#include "qgfx/api/ivertexbuffer.h"
#include "qgfx/context_handle.h"

class VulkanVertexBuffer : public IVertexBuffer
{
	public:
		explicit VulkanVertexBuffer(ContextHandle* context);
		~VulkanVertexBuffer();

		void setData(void* data, const size_t size) override;
		void setLayout(const VertexBufferLayout& layout) override;

		bool construct() override;

		VertexBufferLayout& getLayout() override;

		void bind() override;
		void unbind() override;

	private:
		VkBuffer mBuffer;
		VkDeviceMemory mMemory;
		VertexBufferLayout mLayout;

		VkVertexInputBindingDescription mBindingDescription;
		qtl::vector<VkVertexInputAttributeDescription> mAttributeDescriptions;

		size_t mSize;
		void* mData;
};

#endif // vulkan_vertexbuffer_h__