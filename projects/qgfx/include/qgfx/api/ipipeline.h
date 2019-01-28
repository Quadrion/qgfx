#ifndef ipipeline_h__
#define ipipeline_h__

#include <stdint.h>

#include <qtl/shared_ptr.h>

#include "qgfx/context_handle.h"
#include "qgfx/typedefs.h"

enum class Topology : int32_t
{
	TriangleList,
	TriangleStrip,
	Line,
	Quads,
	Points
};

class IPipeline
{
	public:
		explicit IPipeline(ContextHandle* handle);
		virtual ~IPipeline() = default;

		IPipeline& operator = (const IPipeline&) = delete;

		virtual void construct() = 0;
		virtual void setTopology(const Topology& topology) = 0;
		virtual Shader* addShader() = 0;
	protected:
		ContextHandle* mHandle;
};

#endif // ipipeline_h__