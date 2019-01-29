#ifndef openglcommandpool_h__
#define openglcommandpool_h__

#include "qgfx/api/icommandpool.h"

class OpenGLCommandPool : public ICommandPool
{
	public:
		explicit OpenGLCommandPool(ContextHandle* handle);
		OpenGLCommandPool(const OpenGLCommandPool&) = delete;
		OpenGLCommandPool(OpenGLCommandPool&&) noexcept;

		virtual ~OpenGLCommandPool();

		OpenGLCommandPool& operator=(const OpenGLCommandPool&) = delete;
		OpenGLCommandPool& operator=(OpenGLCommandPool&&) noexcept;

		CommandBuffer* addCommandBuffer() override;
		qtl::vector<CommandBuffer*> getBuffers() override;
		void construct() override;
	private:
		qtl::vector<CommandBuffer*> mBuffers;
};

#endif