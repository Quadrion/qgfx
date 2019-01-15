#ifndef vulkan_vertexbuffer_h__
#define vulkan_vertexbuffer_h__

#include <vulkan/vulkan.h>

#include "qgfx/api/ivertexbuffer.h"
#include "qgfx/context_handle.h"

class VulkanVertexBuffer : public IVertexBuffer
{
	public:
		explicit VulkanVertexBuffer(ContextHandle* context);
		~VulkanVertexBuffer();

		void setData(void* data, const size_t size) override;
		void setLayout(const VertexLayout& layout) override;

		bool construct() override;

		VertexLayout& getLayout() override;

		void bind() override;
		void unbind() override;

	private:
		VkBuffer mBuffer;
		VertexLayout mLayout;

		VkVertexInputBindingDescription mBindingDescription;
		std::vector<VkVertexInputAttributeDescription> mAttributeDescriptions;

		uint32_t mSize;
};

#endif // vulkan_vertexbuffer_h__