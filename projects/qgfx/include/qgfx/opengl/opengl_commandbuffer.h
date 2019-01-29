#ifndef openglcommandbuffer_h__
#define openglcommandbuffer_h__

#include "qgfx/api/icommandbuffer.h"

class OpenGLCommandBuffer : public ICommandBuffer
{
	public:
		explicit OpenGLCommandBuffer(ContextHandle* handle);
		OpenGLCommandBuffer(const OpenGLCommandBuffer&) = delete;
		OpenGLCommandBuffer(OpenGLCommandBuffer&&) noexcept;

		virtual ~OpenGLCommandBuffer() = default;

		OpenGLCommandBuffer& operator=(const OpenGLCommandBuffer&) = delete;
		OpenGLCommandBuffer& operator=(OpenGLCommandBuffer&&) noexcept;

		void record() override;
		void end() override;
	private:
		bool mIsRecording;
};

#endif // openglcommandbuffer_h__