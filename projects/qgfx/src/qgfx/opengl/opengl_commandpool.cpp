#if defined (QGFX_OPENGL)

#include "qgfx/opengl/opengl_commandpool.h"

OpenGLCommandPool::OpenGLCommandPool(ContextHandle* handle)
	: ICommandPool(handle)
{
}

OpenGLCommandPool::OpenGLCommandPool(OpenGLCommandPool&& pool) noexcept
	: ICommandPool(pool.mHandle), mBuffers(qtl::move(pool.mBuffers))
{
	pool.mHandle = nullptr;
	pool.mBuffers.clear();
}

OpenGLCommandPool& OpenGLCommandPool::operator=(OpenGLCommandPool&& other) noexcept
{
	mHandle = other.mHandle;
	mBuffers = qtl::move(other.mBuffers);
	other.mHandle = nullptr;
	other.mBuffers.clear();
	return *this;
}

void OpenGLCommandPool::addCommandBuffer(const qtl::shared_ptr<CommandBuffer>& buffer)
{
	mBuffers.push_back(buffer);
}

void OpenGLCommandPool::construct()
{
}

#endif