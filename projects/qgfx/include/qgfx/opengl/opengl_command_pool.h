#ifndef openglcommandpool_h__
#define openglcommandpool_h__

#include "qgfx/opengl/opengl_command_buffer.h"
#include "qgfx/api/icommandpool.h"

class OpenGLCommandPool : public ICommandPool
{
public:
	explicit OpenGLCommandPool(ContextHandle* handle);
	virtual ~OpenGLCommandPool() = default;

	OpenGLCommandPool& operator=(const OpenGLCommandPool&) = delete;

	void addCommandBuffer(const qtl::shared_ptr<CommandBuffer>& buffer) override;
	void construct() override;
private:
	qtl::vector<qtl::shared_ptr<CommandBuffer>> mBuffers;
};

#endif