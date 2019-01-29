#ifndef iframebuffer_h__
#define iframebuffer_h__

#include "qgfx/context_handle.h"

class IFrameBuffer
{
	public:
		explicit IFrameBuffer(ContextHandle* handle);
		virtual ~IFrameBuffer() = default;

		IFrameBuffer& operator = (const IFrameBuffer&) = delete;

//		virtual void construct(RenderPass* renderPass, attachments, uint32_t attachmentCount, uint32_t width, uint32_t height);

	protected:
		ContextHandle* mHandle;
};

#endif // iframebuffer_h__