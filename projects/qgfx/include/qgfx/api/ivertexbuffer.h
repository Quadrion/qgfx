#ifndef ivertexbuffer_h__
#define ivertexbuffer_h__

#include <stddef.h>
#include <qtl/string.h>
#include <qtl/vector.h>

#include "qgfx/context_handle.h"
#include "qgfx/qassert.h"

struct VertexBufferLayoutElement
{
	qtl::string name;
	uint32_t type;
	size_t size;
	uint32_t count;
	size_t offset;
	bool normalized;
};

class VertexBufferLayout
{
	public:
		VertexBufferLayout() = default;
		~VertexBufferLayout() = default;

		const qtl::vector<VertexBufferLayoutElement>& getLayout() const { return mLayout; }
		size_t getStride() const { return mSize; }

		template<typename T>
		void push(const qtl::string& name, const uint32_t count = 1, const bool normalize = false)
		{
			QGFX_ASSERT(false);
		}
	private:
		size_t mSize = 0;
		qtl::vector<VertexBufferLayoutElement> mLayout;

		void _push(const qtl::string& name, const uint32_t type, const size_t size, const uint32_t count, const bool normalized)
		{
			mLayout.push_back({ name, type, size, count, mSize, normalized });
			mSize += size * count;
		}
};

template<>
inline void VertexBufferLayout::push<float>(const qtl::string& name, const uint32_t count, const bool normalized)
{
	#if defined(QGFX_OPENGL)
	_push(name, GL_FLOAT, sizeof(float), count, normalized);
	#elif defined(QGFX_VULKAN)
	_push(name, VK_FORMAT_R32_SFLOAT, sizeof(float), count, normalized);
	#endif
}

template<>
inline void VertexBufferLayout::push<uint32_t>(const qtl::string& name, const uint32_t count, const bool normalized)
{
	#if defined(QGFX_OPENGL)
		_push(name, GL_UNSIGNED_INT, sizeof(uint32_t), count, normalized);
	#elif defined(QGFX_VULKAN)
		_push(name, VK_FORMAT_R32_UINT, sizeof(uint32_t), count, normalized);
	#endif
}

template<>
inline void VertexBufferLayout::push<uint8_t>(const qtl::string& name, const uint32_t count, const bool normalized)
{
	#if defined(QGFX_OPENGL)
		_push(name, GL_UNSIGNED_BYTE, sizeof(uint8_t), count, normalized);
	#elif defined(QGFX_VULKAN)
		_push(name, VK_FORMAT_R8_UINT, sizeof(uint8_t), count, normalized);
	#endif
}

class IVertexBuffer
{
	public:
		explicit IVertexBuffer(ContextHandle* handle);
		virtual ~IVertexBuffer() = default;

		IVertexBuffer& operator = (const IVertexBuffer&) = delete;

		virtual void setData(void* data, const size_t size) = 0;
		virtual void setLayout(const VertexBufferLayout& layout) = 0;

		virtual bool construct() = 0;

		virtual VertexBufferLayout& getLayout() = 0;

		virtual void bind() = 0;
		virtual void unbind() = 0;
	protected:
		ContextHandle* mHandle;
};

#endif // ivertexbuffer_h__