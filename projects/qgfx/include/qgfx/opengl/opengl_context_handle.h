#ifndef opengl_context_handle_h__
#define opengl_context_handle_h__

#include <glad/glad.h>

#include "qgfx/api/icontexthandle.h"
#include "qgfx/typedefs.h"
#include <qtl/vector.h>

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
		explicit OpenGLContextHandle(Window* window);
		OpenGLContextHandle(const OpenGLContextHandle&) = delete;
		OpenGLContextHandle(OpenGLContextHandle&&) noexcept;

		/// <summary>
		/// Default destructor
		/// </summary>
		~OpenGLContextHandle();

		OpenGLContextHandle& operator=(const OpenGLContextHandle&) = delete;
		OpenGLContextHandle& operator=(OpenGLContextHandle&&) noexcept;

		Pipeline* getPipeline() const override;
		Rasterizer* getRasterizer() const override;

		void initializeGraphics() override;
		void finalizeGraphics() override;

		CommandPool* addCommandPool() override;
		qtl::vector<CommandPool*> getCommandPools() override;
		void startFrame() override;
		void endFrame() override;
		void swap() override;
	private:
		Pipeline* mPipeline;
		Rasterizer* mRasterizer;
		qtl::vector<CommandPool*> mCommandPools;
};

#endif // opengl_context_handle_h__