#ifndef iimageview_h__
#define iimageview_h__

#include "qgfx/context_handle.h"

class IImageView
{
    public:
		explicit IImageView(ContextHandle* handle);
		virtual ~IImageView() = default;

		IImageView& operator = (const IImageView&) = delete;

		virtual void construct(Image2D* image) = 0;

    protected:
		ContextHandle* mHandle;
};

#endif // iimageview_h__