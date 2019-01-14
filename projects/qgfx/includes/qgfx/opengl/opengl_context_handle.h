#ifndef opengl_context_handle_h__
#define opengl_context_handle_h__

#include "GLFW/glfw3.h"

/// <summary>
/// Represents an OpenGL Context Handle.  Does not contain anything
/// as OpenGL does not need a handle.
/// </summary>
class OpenGLContextHandle
{
public:
	/// <summary>
	/// Constructs a new OpenGLContextHandle.  Matches VulkanContextHandle, but the
	/// constructor is a no-op
	/// </summary>
	OpenGLContextHandle(GLFWwindow * window);

	/// <summary>
	/// Default destructor
	/// </summary>
	~OpenGLContextHandle();

	/// <summary>
	/// No-op
	/// </summary>
	/// <returns>
	/// Null pointer
	/// </returns>
	void * getHandle() const;

	/// <summary>
	/// No-op
	/// </summary>
	/// <returns>
	/// Null pointer
	/// </returns>
	void * getPhysicalDevice() const;

	/// <summary>
	/// No-op
	/// </summary>
	/// <returns>
	/// Null pointer
	/// </returns>
	void * getLogicalDevice() const;
};

#endif // opengl_context_handle_h__