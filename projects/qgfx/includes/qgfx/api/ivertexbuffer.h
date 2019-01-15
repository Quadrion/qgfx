#ifndef ivertexbuffer_h__
#define ivertexbuffer_h__

#include "qgfx/context_handle.h"

// TODO (Roderick, Nick): Implement this
struct VertexLayout
{
	
};

class IVertexBuffer
{
	public:
		IVertexBuffer() = default;
		virtual ~IVertexBuffer() = 0;

		IVertexBuffer& operator = (const IVertexBuffer&) = delete;

		virtual void setData(void* data, const size_t size) = 0;
		virtual void setLayout(const VertexLayout& layout) = 0;

		virtual bool construct() = 0;

		virtual VertexLayout& getLayout() = 0;

		virtual void bind() = 0;
		virtual void unbind() = 0;
};

#endif // ivertexbuffer_h__