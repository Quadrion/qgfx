#ifndef icommandbuffer_h__
#define icommandbuffer_h__

#include "qgfx/context_handle.h"
class ICommandBuffer
{
	public:
		explicit ICommandBuffer(ContextHandle* handle);
		virtual ~ICommandBuffer() = default;

		ICommandBuffer& operator = (const ICommandBuffer&) = delete;

	protected:
		ContextHandle* mHandle;
};

#endif // icommandbuffer_h__