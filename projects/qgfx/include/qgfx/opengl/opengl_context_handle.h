#ifndef opengl_context_handle_h__
#define opengl_context_handle_h__

#include <glad/glad.h>
#include "GLFW/glfw3.h"

#include "qgfx/api/icontexthandle.h"
#include "qgfx/typedefs.h"

/// <summary>
/// Represents an OpenGL Context Handle.  Does not contain anything
/// as OpenGL does not need a handle.
/// </summary>
class OpenGLContextHandle : public IContextHandle
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

		Pipeline* getPipeline() const override;
		Rasterizer* getRasterizer() const override;

		void initializeGraphics() override;
		void finalizeGraphics() override;
	private:
		OpenGLPipeline* mPipeline;
		OpenGLRasterizer* mRasterizer;
};

#endif // opengl_context_handle_h__