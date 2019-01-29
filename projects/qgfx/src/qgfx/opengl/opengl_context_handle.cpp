#if defined(QGFX_OPENGL)

#include "qgfx/opengl/opengl_context_handle.h"

#include "qgfx/opengl/opengl_commandpool.h"
#include "qgfx/opengl/opengl_pipeline.h"
#include "qgfx/opengl/opengl_rasterizer.h"
#include "qgfx/opengl/opengl_window.h"

OpenGLContextHandle::OpenGLContextHandle(Window* window)
	: IContextHandle(window)
{
	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	mPipeline = new OpenGLPipeline(this);
	mRasterizer = new OpenGLRasterizer(this);
}

OpenGLContextHandle::OpenGLContextHandle(OpenGLContextHandle&& context) noexcept
	: IContextHandle(context.mWindow), mPipeline(context.mPipeline), mRasterizer(context.mRasterizer), mCommandPools(qtl::move(context.mCommandPools))
{
	context.mPipeline = nullptr;
	context.mRasterizer = nullptr;
	context.mCommandPools.clear();
}

OpenGLContextHandle::~OpenGLContextHandle()
{
    delete mPipeline;
    delete mRasterizer;
	for (auto pool : mCommandPools)
	{
		delete pool;
	}
	mCommandPools.clear();
	mPipeline = nullptr;
	mRasterizer = nullptr;
}

OpenGLContextHandle& OpenGLContextHandle::operator=(OpenGLContextHandle&& handle) noexcept
{
	mWindow = handle.mWindow;
	mPipeline = handle.mPipeline;
	mRasterizer = handle.mRasterizer;
	handle.mPipeline = nullptr;
	handle.mRasterizer = nullptr;
	return *this;
}

Pipeline* OpenGLContextHandle::getPipeline() const
{
	return mPipeline;
}

Rasterizer* OpenGLContextHandle::getRasterizer() const
{
	return mRasterizer;
}

void OpenGLContextHandle::initializeGraphics()
{
	mPipeline->construct();
}

void OpenGLContextHandle::finalizeGraphics()
{
	// no op
}

CommandPool* OpenGLContextHandle::addCommandPool()
{
	auto pool = new CommandPool(this);
	mCommandPools.emplace_back(pool);

	return pool;
}

qtl::vector<CommandPool*> OpenGLContextHandle::getCommandPools()
{
	return mCommandPools;
}

void OpenGLContextHandle::startFrame()
{
}

void OpenGLContextHandle::endFrame()
{
}

void OpenGLContextHandle::swap()
{
	glfwSwapBuffers(reinterpret_cast<GLFWwindow*>(mWindow->getPlatformHandle()));
}

#endif // QGFX_OPENGL