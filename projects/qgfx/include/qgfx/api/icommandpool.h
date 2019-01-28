#ifndef icommandpool_h__
#define icommandpool_h__

#include <qtl/shared_ptr.h>
#include <qtl/vector.h>

#include "qgfx/context_handle.h"
#include "qgfx/typedefs.h"

class ICommandPool
{
	public:
		explicit ICommandPool(ContextHandle* handle);
		virtual ~ICommandPool() = default;

		ICommandPool& operator = (const ICommandPool&) = delete;

		virtual CommandBuffer* addCommandBuffer() = 0;
		virtual qtl::vector<CommandBuffer*> getBuffers() = 0;
		virtual void construct() = 0;

	protected:
		ContextHandle* mHandle;
};

#endif // icommandpool_h__