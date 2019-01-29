#ifndef iimage_h__
#define iimage_h__

#include "qgfx/context_handle.h"

enum class ImageFormat : uint32_t
{
	Red,
	Green,
	Blue,
	Alpha,
	RGB,
	RGBA,
	Luminance,
	LuminanceAlpha
};

enum class ImageDataType : uint32_t
{
	Byte,
	UByte,
	Short,
	UShort,
	Int,
	UInt,
	Float,
	UShort4_4_4_4,
	UShort5_5_5_1,
	UInt8_8_8_8,
	UInt10_10_10_2
};

class IImage2D
{
	public:
		explicit IImage2D(ContextHandle* handle);
		virtual ~IImage2D() = default;

		IImage2D& operator = (const IImage2D&) = delete;

		virtual void construct(const uint32_t width, const uint32_t height, const uint8_t bpp, const ImageFormat& format, const ImageDataType& type) = 0;
		virtual void setData(const uint8_t* data, const uint32_t dataSize) = 0;

	protected:
		ContextHandle* mHandle;
};

#endif // iimage_h__