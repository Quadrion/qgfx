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

OpenGLContextHandle::~OpenGLContextHandle()
{
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

#endif // QGFX_OPENGL