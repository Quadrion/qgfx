#ifndef openglcommandbuffer_h__
#define openglcommandbuffer_h__

#include "qgfx/api/icommandbuffer.h"

class OpenGLCommandBuffer : public ICommandBuffer
{
	public:
		explicit OpenGLCommandBuffer(ContextHandle* handle);
		virtual ~OpenGLCommandBuffer() = default;
};

#endif // openglcommandbuffer_h__