#ifndef vulkan_commandbuffer_h__
#define vulkan_commandbuffer_h__

#include "qgfx/api/icommandbuffer.h"
#include "qgfx/context_handle.h"

class VulkanCommandBuffer : public ICommandBuffer
{
	public:
		explicit VulkanCommandBuffer(ContextHandle* context);
		~VulkanCommandBuffer();
};

#endif // vulkan_commandbuffer_h__