#ifndef irenderpass_h__
#define irenderpass_h__

#include "qgfx/context_handle.h"
#include "qgfx/typedefs.h"

class IRenderPass
{
	public:
		explicit IRenderPass(ContextHandle* handle);
		virtual ~IRenderPass() = default;

		IRenderPass& operator = (const IRenderPass&) = delete;

	protected:
		ContextHandle* mHandle;
};

#endif // irenderpass_h__