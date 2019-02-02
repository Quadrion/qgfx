#ifndef openglimage2d_h__
#define openglimage2d_h__

#include "qgfx/api/iimage2d.h"

class OpenGLImage2D : public IImage2D
{
    public:
	    explicit OpenGLImage2D(ContextHandle* handle);
	    OpenGLImage2D(const OpenGLImage2D&) = delete;
	    OpenGLImage2D(OpenGLImage2D&&) noexcept;

	    ~OpenGLImage2D();

	    OpenGLImage2D& operator=(const OpenGLImage2D&) = delete;
	    OpenGLImage2D& operator=(OpenGLImage2D&&) noexcept;

	    void construct(const uint32_t width, const uint32_t height, const uint8_t bpp, const ImageFormat& format, const ImageDataType& type, const ImageType& imageType) override;
	    void setData(const uint8_t* data, const uint32_t dataSize) override;
		void* getImageHandle() const override;
	private:
	    GLuint mId;

	    uint32_t mWidth, mHeight;
	    uint8_t mBpp;

	    ImageFormat mFormat;
	    ImageDataType mDataType;
};

constexpr GLenum getInternalFormat(const ImageFormat format, const ImageDataType type);
constexpr GLenum getFormat(const ImageFormat& format);
constexpr GLenum getType(const ImageDataType& type);

#endif