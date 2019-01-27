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

		virtual void addCommandBuffer(const qtl::shared_ptr<CommandBuffer>& buffer) = 0;

		virtual void construct() = 0;

	protected:
		qtl::vector<qtl::shared_ptr<CommandBuffer>> mBuffers;
		ContextHandle* mHandle;
};

#endif // icommandpool_h__