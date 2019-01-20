#if defined(QGFX_OPENGL)

#include "qgfx/opengl/opengl_context_handle.h"

OpenGLContextHandle::OpenGLContextHandle(GLFWwindow* window)
{
}

OpenGLContextHandle::~OpenGLContextHandle()
{
}

void* OpenGLContextHandle::getHandle() const
{
	return nullptr;
}

void* OpenGLContextHandle::getPhysicalDevice() const
{
	return nullptr;
}

void* OpenGLContextHandle::getLogicalDevice() const
{
	return nullptr;
}

#endif // QGFX_OPENGL