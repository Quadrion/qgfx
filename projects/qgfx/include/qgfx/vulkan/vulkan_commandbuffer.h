#ifndef vulkan_commandbuffer_h__
#define vulkan_commandbuffer_h__

#include "qgfx/api/icommandbuffer.h"
#include "qgfx/context_handle.h"

class VulkanCommandBuffer : public ICommandBuffer
{
	public:
		explicit VulkanCommandBuffer(ContextHandle* handle);
		~VulkanCommandBuffer();

		void record() override;
		void end() override;

		VkCommandBuffer getBuffer() const;

	private:
		friend class VulkanCommandPool;

		VkCommandBuffer mBuffer;
};

#endif // vulkan_commandbuffer_h__