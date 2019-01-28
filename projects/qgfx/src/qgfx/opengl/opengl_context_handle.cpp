#if defined(QGFX_OPENGL)

#include "qgfx/opengl/opengl_context_handle.h"

#include "qgfx/opengl/opengl_pipeline.h"
#include "qgfx/opengl/opengl_rasterizer.h"

OpenGLContextHandle::OpenGLContextHandle(GLFWwindow* window)
	: IContextHandle(window)
{
	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	mPipeline = new OpenGLPipeline(this);
	mRasterizer = new OpenGLRasterizer(this);
}

OpenGLContextHandle::OpenGLContextHandle(OpenGLContextHandle&& context) noexcept
	: IContextHandle(context.mWindow), mPipeline(context.mPipeline), mRasterizer(context.mRasterizer)
{
	context.mPipeline = nullptr;
	context.mRasterizer = nullptr;
}

OpenGLContextHandle::~OpenGLContextHandle()
{
	if (mPipeline) delete mPipeline;
	if (mRasterizer) delete mRasterizer;
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

Pipeline * OpenGLContextHandle::getPipeline() const
{
	return mPipeline;
}

Rasterizer * OpenGLContextHandle::getRasterizer() const
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

#endif // QGFX_OPENGL