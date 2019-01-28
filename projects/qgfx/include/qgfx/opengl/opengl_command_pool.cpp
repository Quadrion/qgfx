#if defined (QGFX_OPENGL)

#include "opengl_command_pool.h"

OpenGLCommandPool::OpenGLCommandPool(ContextHandle * handle)
	: ICommandPool(handle)
{
}

void OpenGLCommandPool::addCommandBuffer(const qtl::shared_ptr<CommandBuffer>& buffer)
{
	mBuffers.push_back(buffer);
}

void OpenGLCommandPool::construct()
{
}

#endif