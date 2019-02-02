#ifndef vulkan_imageview_h__
#define vulkan_imageview_h__

#include <vulkan/vulkan.h>

#include "qgfx/api/iimageview.h"

class VulkanImageView : public IImageView
{
    public:
		explicit VulkanImageView(ContextHandle* handle);
		~VulkanImageView();

		void construct(Image2D* image) override;

    private:
		VkImageView mImageView;
};

#endif // vulkan_imageview_h__