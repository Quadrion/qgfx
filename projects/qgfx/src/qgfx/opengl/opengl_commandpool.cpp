#if defined (QGFX_OPENGL)

#include "qgfx/opengl/opengl_commandpool.h"

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