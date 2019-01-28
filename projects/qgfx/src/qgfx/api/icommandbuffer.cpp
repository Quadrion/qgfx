#include "qgfx/api/icommandbuffer.h"

ICommandBuffer::ICommandBuffer(ContextHandle* handle, CommandPool* pool)
	: mHandle(handle), mPool(pool)
{
	
}