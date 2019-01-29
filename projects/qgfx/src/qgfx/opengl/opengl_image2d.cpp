#if defined (QGFX_OPENGL)

#include "qgfx/opengl/opengl_image2d.h"
#include "qgfx/qassert.h"

#include "qtl/utility.h"

static constexpr GLenum getInternalFormat(const ImageFormat format, const ImageDataType type)
{
	switch (format)
	{
	case ImageFormat::Red:
	{
		switch (type)
		{
			case ImageDataType::Byte: return GL_R8I;
			case ImageDataType::UByte: return GL_R8UI;
			case ImageDataType::Short: return GL_R16I;
			case ImageDataType::UShort: return GL_R16UI;
			case ImageDataType::Int: return GL_R32I;
			case ImageDataType::UInt: return GL_R32UI;
			case ImageDataType::Float: return GL_R32F;
			default: return 0;
		}
	}
	case ImageFormat::Green:
	{
		switch (type)
		{
			case ImageDataType::Byte: return GL_R8I;
			case ImageDataType::UByte: return GL_R8UI;
			case ImageDataType::Short: return GL_R16I;
			case ImageDataType::UShort: return GL_R16UI;
			case ImageDataType::Int: return GL_R32I;
			case ImageDataType::UInt: return GL_R32UI;
			case ImageDataType::Float: return GL_R32F;
			default: return 0;
		}
	}
	case ImageFormat::Blue:
	{
		switch (type)
		{
			case ImageDataType::Byte: return GL_R8I;
			case ImageDataType::UByte: return GL_R8UI;
			case ImageDataType::Short: return GL_R16I;
			case ImageDataType::UShort: return GL_R16UI;
			case ImageDataType::Int: return GL_R32I;
			case ImageDataType::UInt: return GL_R32UI;
			case ImageDataType::Float: return GL_R32F;
			default: return 0;
		}
	}
	case ImageFormat::Alpha:
	{
		switch (type)
		{
			case ImageDataType::Byte: return GL_R8I;
			case ImageDataType::UByte: return GL_R8UI;
			case ImageDataType::Short: return GL_R16I;
			case ImageDataType::UShort: return GL_R16UI;
			case ImageDataType::Int: return GL_R32I;
			case ImageDataType::UInt: return GL_R32UI;
			case ImageDataType::Float: return GL_R32F;
			default: return 0;
		}
	}
	case ImageFormat::RGB:
	{
		switch (type)
		{
			case ImageDataType::Byte: return GL_RGB8I;
			case ImageDataType::UByte: return GL_RGB8UI;
			case ImageDataType::Short: return GL_RGB16I;
			case ImageDataType::UShort: return GL_RGB16UI;
			case ImageDataType::Int: return GL_RGB32I;
			case ImageDataType::UInt: return GL_RGB32UI;
			case ImageDataType::Float: return GL_RGB32F;
			default: return 0;
		}
	}
	case ImageFormat::RGBA:
	{
		switch (type)
		{
			case ImageDataType::Byte: return GL_RGBA8I;
			case ImageDataType::UByte: return GL_RGBA8UI;
			case ImageDataType::Short: return GL_RGBA16I;
			case ImageDataType::UShort: return GL_RGBA16UI;
			case ImageDataType::Int: return GL_RGBA32I;
			case ImageDataType::UInt: return GL_RGBA32UI;
			case ImageDataType::Float: return GL_RGBA32F;
			case ImageDataType::UShort4_4_4_4: return GL_RGBA4;
			case ImageDataType::UShort5_5_5_1: return GL_RGB5_A1;
			case ImageDataType::UInt8_8_8_8: return GL_RGBA8;
			case ImageDataType::UInt10_10_10_2: return GL_RGB10_A2;
			default: return 0;
		}
	}
	case ImageFormat::Luminance:
	{
		switch (type)
		{
			case ImageDataType::Byte: return GL_R8_SNORM;
			case ImageDataType::UByte: return GL_R8;
			case ImageDataType::Short: return GL_R16F;
			case ImageDataType::UShort: return GL_R16F;
			case ImageDataType::Int: return GL_R32F;
			case ImageDataType::UInt: return GL_R32F;
			case ImageDataType::Float: return GL_R32F;
			default: return 0;
		}
	}
	case ImageFormat::LuminanceAlpha:
	{
		switch (type)
		{
			case ImageDataType::Byte: return GL_R8_SNORM;
			case ImageDataType::UByte: return GL_R8;
			case ImageDataType::Short: return GL_R16F;
			case ImageDataType::UShort: return GL_R16F;
			case ImageDataType::Int: return GL_R32F;
			case ImageDataType::UInt: return GL_R32F;
			case ImageDataType::Float: return GL_R32F;
			default: return 0;
		}
	}
	default: return 0;
	}
}

static constexpr GLenum getFormat(const ImageFormat& format)
{
	switch (format)
	{
		case ImageFormat::Red: return GL_RED;
		case ImageFormat::Green: return GL_GREEN;
		case ImageFormat::Blue: return GL_BLUE;
		case ImageFormat::Alpha: return GL_ALPHA;
		case ImageFormat::RGB: return GL_RGB;
		case ImageFormat::RGBA: return GL_RGBA;
		case ImageFormat::Luminance: return GL_RED;
		case ImageFormat::LuminanceAlpha: return GL_RG;
	}
	
	return 0;
}

static constexpr GLenum getType(const ImageDataType& type)
{
	switch (type)
	{
		case ImageDataType::Byte: return GL_BYTE;
		case ImageDataType::UByte: return GL_UNSIGNED_BYTE;
		case ImageDataType::Short: return GL_SHORT;
		case ImageDataType::UShort: return GL_UNSIGNED_SHORT;
		case ImageDataType::Int: return GL_INT;
		case ImageDataType::UInt: return GL_UNSIGNED_INT;
		case ImageDataType::Float: return GL_FLOAT;
		case ImageDataType::UShort4_4_4_4: return GL_UNSIGNED_SHORT_4_4_4_4;
		case ImageDataType::UShort5_5_5_1: return GL_UNSIGNED_SHORT_5_5_5_1;
		case ImageDataType::UInt8_8_8_8: return GL_UNSIGNED_INT_8_8_8_8;
		case ImageDataType::UInt10_10_10_2: return GL_UNSIGNED_INT_10_10_10_2;
	}

	return 0;
}

OpenGLImage2D::OpenGLImage2D(ContextHandle* handle)
	: IImage2D(handle), mId(0), mWidth(0), mHeight(0), mBpp(0), mFormat(ImageFormat::Red), mDataType(ImageDataType::Byte)
{
}

OpenGLImage2D::OpenGLImage2D(OpenGLImage2D&& image) noexcept
	: IImage2D(image.mHandle), mId(image.mId), mWidth(image.mWidth), mHeight(image.mHeight), mBpp(image.mBpp),
      mFormat(image.mFormat), mDataType(image.mDataType)
{
	image.mId = 0;
}

OpenGLImage2D::~OpenGLImage2D()
{
	if (mId)
	{
		glDeleteTextures(1, &mId);
	}
	mId = 0;
}

OpenGLImage2D& OpenGLImage2D::operator=(OpenGLImage2D&& image) noexcept
{
	mHandle = qtl::move(image.mHandle);
	mWidth = qtl::move(image.mWidth);
	mHeight = qtl::move(image.mHeight);
	mBpp = qtl::move(image.mBpp);
	mId = qtl::move(image.mId);
	image.mId = 0;

	return *this;
}

void OpenGLImage2D::construct(const uint32_t width, const uint32_t height, const uint8_t bpp, const ImageFormat& format, const ImageDataType& type)
{
	glGenTextures(1, &mId);
	const GLenum glFormat = getInternalFormat(format, type);
	QGFX_ASSERT_MSG(glFormat != 0, "Could not determine internal OpenGL format from format and type provided.\n");
	glTexStorage2D(mId, 1, glFormat, width, height);
	mWidth = width;
	mHeight = height;
	mBpp = bpp;
	mFormat = format;
	mDataType = type;
}

void OpenGLImage2D::setData(const uint8_t* data, const uint32_t dataSize)
{
	glTextureSubImage2D(mId, 0, 0, 0, mWidth, mHeight, getFormat(mFormat), getType(mDataType), data);
}

#endif


