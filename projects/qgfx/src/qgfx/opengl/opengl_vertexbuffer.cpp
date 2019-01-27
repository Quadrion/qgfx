#if defined(QGFX_OPENGL)

#include "qgfx/opengl/opengl_vertexbuffer.h"
#include "qgfx/qassert.h"

#include <cstring>

OpenGLVertexBuffer::OpenGLVertexBuffer(ContextHandle* handle)
	: IVertexBuffer(handle), mId(0)
{
}

OpenGLVertexBuffer::OpenGLVertexBuffer(OpenGLVertexBuffer && buf) noexcept
	: IVertexBuffer(mHandle), mId(buf.mId), mData(buf.mData)
{
	buf.mId = 0;
	buf.mData = nullptr;
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
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

OpenGLVertexBuffer& OpenGLVertexBuffer::operator=(OpenGLVertexBuffer && buf) noexcept
{
	mId = buf.mId;
	mData = buf.mData;

	buf.mId = 0;
	buf.mData = nullptr;

	return *this;
}

void OpenGLVertexBuffer::setData(void * data, const size_t size)
{
	mData = new char[size];
	memcpy(mData, data, size);
}

void OpenGLVertexBuffer::setLayout(const VertexBufferLayout & layout)
{
	mLayout = layout;
}

bool OpenGLVertexBuffer::construct()
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

		glVertexAttribPointer(idx, ct, tp, nm, static_cast<GLsizei>(mLayout.getStride()), reinterpret_cast<const void*>(of));
		glEnableVertexAttribArray(idx);
		++idx;
	}
	glBindBuffer(GL_VERTEX_ARRAY, 0);
	return true;
}

VertexBufferLayout & OpenGLVertexBuffer::getLayout()
{
	return mLayout;
}

void OpenGLVertexBuffer::bind()
{
	glBindBuffer(GL_VERTEX_ARRAY, mId);
}

void OpenGLVertexBuffer::unbind()
{
	glBindBuffer(GL_VERTEX_ARRAY, 0);
}

#endif