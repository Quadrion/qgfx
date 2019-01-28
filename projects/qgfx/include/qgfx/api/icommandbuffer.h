#ifndef icommandbuffer_h__
#define icommandbuffer_h__

#include "qgfx/context_handle.h"

#include "qgfx/typedefs.h"

class ICommandBuffer
{
	public:
		explicit ICommandBuffer(ContextHandle* handle, CommandPool* pool);
		virtual ~ICommandBuffer() = default;

		ICommandBuffer& operator = (const ICommandBuffer&) = delete;

		virtual void record() = 0;
		virtual void end() = 0;

	protected:
		ContextHandle* mHandle;
		CommandPool* mPool;
};

#endif // icommandbuffer_h__