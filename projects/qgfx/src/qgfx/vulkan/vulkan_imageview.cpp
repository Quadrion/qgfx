#if defined(QGFX_VULKAN)

#include "qgfx/vulkan/vulkan_imageview.h"
#include "qgfx/vulkan/vulkan_image2d.h"

constexpr VkImageAspectFlags convertQgfxImageTypeToVulkan(const ImageType type)
{
    if(type & ImageType::Color)
    {
        
    }
}

VulkanImageView::VulkanImageView(ContextHandle* handle) : IImageView(handle)
{
	mImageView = nullptr;
}

VulkanImageView::~VulkanImageView()
{
    
}

void VulkanImageView::construct(Image2D* image)
{
	VkImageViewCreateInfo viewInfo = {};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = static_cast<VkImage>(image->getImageHandle());
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = convertQgfxFormatToVulkan(image->getImageFormat(), image->getImageDataType());
	viewInfo.subresourceRange.aspectMask = image->getImageType();
}

#endif // QGFX_VULKAN