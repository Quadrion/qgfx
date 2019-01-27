#ifndef opengl_vertexbuffer_h__
#define opengl_vertexbuffer_h__

#include <cstdint>
#include <glad/glad.h>

#include "qgfx/api/ivertexbuffer.h"

class OpenGLVertexBuffer : public IVertexBuffer
{
	public:
		OpenGLVertexBuffer(ContextHandle* handle);
		OpenGLVertexBuffer(const OpenGLVertexBuffer&) = delete;
		OpenGLVertexBuffer(OpenGLVertexBuffer&& buf) noexcept;
		~OpenGLVertexBuffer();

		OpenGLVertexBuffer& operator=(const OpenGLVertexBuffer&) = delete;
		OpenGLVertexBuffer& operator=(OpenGLVertexBuffer&& buf) noexcept;

		void setData(void * data, const size_t size) override;
		void setLayout(const VertexBufferLayout& layout) override;
		bool construct() override;

		VertexBufferLayout& getLayout() override;
		void bind() override;
		void unbind() override;
	private:
		GLuint mId;
		VertexBufferLayout mLayout;
		void * mData = nullptr;
		std::size_t mSize = 0;
};

#endif // opengl_vertexbuffer_h__