#if defined (QGFX_OPENGL)

#include "qgfx/opengl/opengl_imageview.h"

OpenGLImageView::OpenGLImageView(ContextHandle* handle)
	: IImageView(handle)
{	}

OpenGLImageView::OpenGLImageView(OpenGLImageView&& image) noexcept
	: IImageView(image.mHandle)
{
	mImage = image.mImage;
	image.mImage = nullptr;
}

OpenGLImageView & OpenGLImageView::operator=(OpenGLImageView && image) noexcept
{
	mHandle = image.mHandle;
	mImage = image.mImage;
	image.mImage = nullptr;
	return *this;
}

void OpenGLImageView::construct(Image2D* image)
{
	mImage = image;
}

#endif