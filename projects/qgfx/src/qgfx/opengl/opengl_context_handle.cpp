#ifndef opengl_context_handle__
#define opengl_context_handle__

#include "qgfx/opengl/opengl_context_handle.h"

OpenGLContextHandle::OpenGLContextHandle(GLFWwindow* window)
{
}

OpenGLContextHandle::~OpenGLContextHandle()
{
}

void * OpenGLContextHandle::getHandle() const
{
	return nullptr;
}

void * OpenGLContextHandle::getPhysicalDevice() const
{
	return nullptr;
}

void * OpenGLContextHandle::getLogicalDevice() const
{
	return nullptr;
}

#endif