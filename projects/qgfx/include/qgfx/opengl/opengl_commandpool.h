#ifndef openglcommandpool_h__
#define openglcommandpool_h__

#include "qgfx/opengl/opengl_commandbuffer.h"
#include "qgfx/api/icommandpool.h"

class OpenGLCommandPool : public ICommandPool
{
public:
	explicit OpenGLCommandPool(ContextHandle* handle);
	OpenGLCommandPool(const OpenGLCommandPool&) = delete;
	OpenGLCommandPool(OpenGLCommandPool&& pool) noexcept;

	virtual ~OpenGLCommandPool() = default;

	OpenGLCommandPool& operator=(const OpenGLCommandPool&) = delete;
	OpenGLCommandPool& operator=(OpenGLCommandPool&& other) noexcept;

	void addCommandBuffer(const qtl::shared_ptr<CommandBuffer>& buffer) override;
	void construct() override;
private:
	qtl::vector<qtl::shared_ptr<CommandBuffer>> mBuffers;
};

#endif