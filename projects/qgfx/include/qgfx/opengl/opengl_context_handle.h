#ifndef opengl_context_handle_h__
#define opengl_context_handle_h__

#include <glad/glad.h>
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
		explicit OpenGLContextHandle(GLFWwindow * window);

		/// <summary>
		/// Default destructor
		/// </summary>
		~OpenGLContextHandle();
};

#endif // opengl_context_handle_h__