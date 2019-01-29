#if defined (QGFX_OPENGL)

#include "qgfx/opengl/opengl_commandbuffer.h"

OpenGLCommandBuffer::OpenGLCommandBuffer(ContextHandle* handle)
	: ICommandBuffer(handle), mIsRecording(false)
{
}

OpenGLCommandBuffer::OpenGLCommandBuffer(OpenGLCommandBuffer&& buf) noexcept
	: ICommandBuffer(buf.mHandle), mIsRecording(buf.mIsRecording)
{
	buf.mHandle = nullptr;
}

OpenGLCommandBuffer& OpenGLCommandBuffer::operator=(OpenGLCommandBuffer&& buf) noexcept
{
	mHandle = buf.mHandle;
	buf.mHandle = nullptr;
	mIsRecording = buf.mIsRecording;
	return *this;
}

void OpenGLCommandBuffer::record()
{
	mIsRecording = true;
}

void OpenGLCommandBuffer::end()
{
	mIsRecording = false;
}

#endif
