#if defined(QGFX_OPENGL)

#include "qgfx/opengl/opengl_vertexbuffer.h"
#include "qgfx/qassert.h"

#include <cstring>

VertexBuffer::VertexBuffer()
	: mId(0)
{
}

VertexBuffer::VertexBuffer(VertexBuffer && buf) noexcept
	: mId(buf.mId), mData(buf.mData)
{
	buf.mId = 0;
	buf.mData = nullptr;
}

VertexBuffer::~VertexBuffer()
{
	if (mData)
	{
		delete[] reinterpret_cast<char*>(mData);
		mData = nullptr;
	}
	if (mId)
	{
		glDeleteBuffers(1, &mId);
		mId = 0;
	}
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer && buf) noexcept
{
	mId = buf.mId;
	mData = buf.mData;

	buf.mId = 0;
	buf.mData = nullptr;

	return *this;
}

void VertexBuffer::setData(void * data, const size_t size)
{
	mData = new char[size];
	memcpy(mData, data, size);
}

void VertexBuffer::setLayout(const VertexBufferLayout & layout)
{
	mLayout = layout;
}

bool VertexBuffer::construct()
{
	QGFX_ASSERT_MSG(mId != 0, "Vertex Buffer already constructed.");
	glCreateBuffers(1, &mId);
	if (mId == 0)
	{
		return false;
	}
	glBindBuffer(GL_VERTEX_ARRAY, mId);
	glBufferStorage(GL_VERTEX_ARRAY, mSize, mData, 0);
	GLuint idx = 0;
	for (auto format : mLayout.getLayout())
	{
		auto sz = format.size;
		auto ct = format.count;
		auto tp = format.type;
		auto of = format.offset;
		auto nm = format.normalized;

		glVertexAttribPointer(idx, ct, tp, nm, mLayout.getStride(), reinterpret_cast<const void*>(of));
		glEnableVertexAttribArray(idx);
		++idx;
	}
	glBindBuffer(GL_VERTEX_ARRAY, 0);
	return true;
}

VertexBufferLayout & VertexBuffer::getLayout()
{
	return mLayout;
}

void VertexBuffer::bind()
{
	glBindBuffer(GL_VERTEX_ARRAY, mId);
}

void VertexBuffer::unbind()
{
	glBindBuffer(GL_VERTEX_ARRAY, 0);
}

#endif