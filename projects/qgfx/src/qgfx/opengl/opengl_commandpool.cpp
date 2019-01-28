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

OpenGLCommandPool::~OpenGLCommandPool()
{
	for (auto buf : mBuffers)
	{
		delete buf;
	}
	mBuffers.clear();
}

OpenGLCommandPool& OpenGLCommandPool::operator=(OpenGLCommandPool&& other) noexcept
{
	mHandle = other.mHandle;
	mBuffers = qtl::move(other.mBuffers);
	other.mHandle = nullptr;
	other.mBuffers.clear();
	return *this;
}

CommandBuffer * OpenGLCommandPool::addCommandBuffer()
{
	auto buf = new CommandBuffer(mHandle);
	mBuffers.push_back(buf);

	return buf;
}

qtl::vector<CommandBuffer*> OpenGLCommandPool::getBuffers()
{
	return mBuffers;
}

void OpenGLCommandPool::construct()
{
}

#endif