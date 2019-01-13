#ifndef opengl_context_handle_h__
#define opengl_context_handle_h__

#include "GLFW/glfw3.h"

class OpenGLContextHandle
{
public:
	OpenGLContextHandle(GLFWwindow * window);
	~OpenGLContextHandle();

	void * getHandle() const;
	void * getPhysicalDevice() const;
	void * getLogicalDevice() const;
};

#endif // opengl_context_handle_h__