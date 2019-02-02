#ifndef opengl_imageview_h__
#define opengl_imageview_h__

#include "qgfx/api/iimageview.h"

class OpenGLImageView : public IImageView
{
public:
	OpenGLImageView(ContextHandle* handle);
	OpenGLImageView(const OpenGLImageView&) = delete;
	OpenGLImageView(OpenGLImageView&& image) noexcept;

	OpenGLImageView& operator=(const OpenGLImageView&) = delete;
	OpenGLImageView& operator=(OpenGLImageView&& image) noexcept;

	void construct(Image2D* image) override;
private:
	Image2D* mImage;
};

#endif // opengl_imageview_h__