#ifndef vulkan_image2d_h__
#define vulkan_image2d_h__

#include "qgfx/api/iimage2d.h"

class VulkanImage2D : public IImage2D
{
	public:
		explicit VulkanImage2D(ContextHandle* handle);
		~VulkanImage2D();

		void construct(const uint32_t width, const uint32_t height, const uint8_t bpp, const ImageFormat& format, const ImageDataType& type, const ImageType& imageType) override;
		void setData(const uint8_t* data, const uint32_t dataSize) override;

		void* getImageHandle() const override;

	private:
		VkImage mImage;
		VkDeviceMemory mMemory;
		VkFormat mFormat;
		VkDeviceSize mImageSize;

		uint32_t mWidth, mHeight;
		uint8_t mBpp;
};

constexpr VkFormat convertQgfxFormatToVulkan(const ImageFormat format, const ImageDataType type);

#endif // vulkan_image2d_h__