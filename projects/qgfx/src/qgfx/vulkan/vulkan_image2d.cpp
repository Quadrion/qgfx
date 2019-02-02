#if defined(QGFX_VULKAN)
#include "qgfx/qassert.h"
#include "qgfx/vulkan/vulkan_memory.h"

#include "qgfx/vulkan/vulkan_image2d.h"

constexpr VkFormat convertQgfxFormatToVulkan(const ImageFormat format, const ImageDataType type)
{
	switch(format)
	{
		case ImageFormat::Red:
		{
			switch(type)
			{
				case ImageDataType::Byte: return VK_FORMAT_R8_SINT;
				case ImageDataType::UByte: return VK_FORMAT_R8_UINT;
				case ImageDataType::Short: return VK_FORMAT_R16_SINT;
				case ImageDataType::UShort: return VK_FORMAT_R16_UINT;
				case ImageDataType::Int: return VK_FORMAT_R32_SINT;
				case ImageDataType::UInt: return VK_FORMAT_R32_UINT;
				case ImageDataType::Float: return VK_FORMAT_R32_SFLOAT;
				default: return VK_FORMAT_UNDEFINED;
			}
		}
		case ImageFormat::Green:
		{
			switch (type)
			{
			    case ImageDataType::Byte: return VK_FORMAT_R8_SINT;
			    case ImageDataType::UByte: return VK_FORMAT_R8_UINT;
			    case ImageDataType::Short: return VK_FORMAT_R16_SINT;
			    case ImageDataType::UShort: return VK_FORMAT_R16_UINT;
			    case ImageDataType::Int: return VK_FORMAT_R32_SINT;
			    case ImageDataType::UInt: return VK_FORMAT_R32_UINT;
			    case ImageDataType::Float: return VK_FORMAT_R32_SFLOAT;
				default: return VK_FORMAT_UNDEFINED;
			}
		}
		case ImageFormat::Blue:
		{
			switch (type)
			{
			    case ImageDataType::Byte: return VK_FORMAT_R8_SINT;
			    case ImageDataType::UByte: return VK_FORMAT_R8_UINT;
			    case ImageDataType::Short: return VK_FORMAT_R16_SINT;
			    case ImageDataType::UShort: return VK_FORMAT_R16_UINT;
			    case ImageDataType::Int: return VK_FORMAT_R32_SINT;
			    case ImageDataType::UInt: return VK_FORMAT_R32_UINT;
			    case ImageDataType::Float: return VK_FORMAT_R32_SFLOAT;
				default: return VK_FORMAT_UNDEFINED;
			}
		}
		case ImageFormat::Alpha:
		{
			switch (type)
			{
			    case ImageDataType::Byte: return VK_FORMAT_R8_SINT;
			    case ImageDataType::UByte: return VK_FORMAT_R8_UINT;
			    case ImageDataType::Short: return VK_FORMAT_R16_SINT;
			    case ImageDataType::UShort: return VK_FORMAT_R16_UINT;
			    case ImageDataType::Int: return VK_FORMAT_R32_SINT;
			    case ImageDataType::UInt: return VK_FORMAT_R32_UINT;
			    case ImageDataType::Float: return VK_FORMAT_R32_SFLOAT;
				default: return VK_FORMAT_UNDEFINED;
			}
		}
		case ImageFormat::RGB:
		{
			switch (type)
			{
				case ImageDataType::Byte: return VK_FORMAT_R8G8B8_SINT;
				case ImageDataType::UByte: return VK_FORMAT_R8G8B8_UINT;
				case ImageDataType::Short: return VK_FORMAT_R16G16B16_SINT;
				case ImageDataType::UShort: return VK_FORMAT_R16G16B16_UINT;
				case ImageDataType::Int: return VK_FORMAT_R32G32B32_SINT;
				case ImageDataType::UInt: return VK_FORMAT_R32G32B32_UINT;
				case ImageDataType::Float: return VK_FORMAT_R32G32B32_SFLOAT;
				default: return VK_FORMAT_UNDEFINED;
			}
		}
		case ImageFormat::RGBA:
		{
			switch (type)
			{
				case ImageDataType::Byte: return VK_FORMAT_R8G8B8A8_SINT;
				case ImageDataType::UByte: return VK_FORMAT_R8G8B8A8_UINT;
				case ImageDataType::Short: return VK_FORMAT_R16G16B16A16_SINT;
				case ImageDataType::UShort: return VK_FORMAT_R16G16B16A16_UINT;
				case ImageDataType::Int: return VK_FORMAT_R32G32B32A32_SINT;
				case ImageDataType::UInt: return VK_FORMAT_R32G32B32A32_UINT;
				case ImageDataType::Float: return VK_FORMAT_R32G32B32A32_SFLOAT;
				case ImageDataType::UShort4_4_4_4: return VK_FORMAT_R4G4B4A4_UNORM_PACK16;
				case ImageDataType::UShort5_5_5_1: return VK_FORMAT_R5G5B5A1_UNORM_PACK16;
				case ImageDataType::UInt8_8_8_8: return VK_FORMAT_R8G8B8A8_UINT;
				case ImageDataType::UInt10_10_10_2: return VK_FORMAT_A2R10G10B10_UINT_PACK32;
				default: return VK_FORMAT_UNDEFINED;
			}
		}
		case ImageFormat::Luminance:
		{
			switch (type)
			{
				case ImageDataType::Byte: return VK_FORMAT_R8_SNORM;
				case ImageDataType::UByte: return VK_FORMAT_R8_UNORM;
				case ImageDataType::Short: return VK_FORMAT_R16_SFLOAT;
				case ImageDataType::UShort: return VK_FORMAT_R16_SFLOAT;
				case ImageDataType::Int: return VK_FORMAT_R32_SFLOAT;
				case ImageDataType::UInt: return VK_FORMAT_R32_SFLOAT;
				case ImageDataType::Float: return VK_FORMAT_R32_SFLOAT;
				default: return VK_FORMAT_UNDEFINED;
			}
		}
		case ImageFormat::LuminanceAlpha:
		{
			switch (type)
			{
				case ImageDataType::Byte: return VK_FORMAT_R8G8_SNORM;
				case ImageDataType::UByte: return VK_FORMAT_R8G8_UNORM;
				case ImageDataType::Short: return VK_FORMAT_R16G16_SFLOAT;
				case ImageDataType::UShort: return VK_FORMAT_R16G16_SFLOAT;
				case ImageDataType::Int: return VK_FORMAT_R32G32_SFLOAT;
				case ImageDataType::UInt: return VK_FORMAT_R32G32_SFLOAT;
				case ImageDataType::Float: return VK_FORMAT_R32G32_SFLOAT;
				default: return VK_FORMAT_UNDEFINED;
			}
		}
		default: return VK_FORMAT_UNDEFINED;
	}
}

VulkanImage2D::VulkanImage2D(ContextHandle* handle) : IImage2D(handle)
{
	mImage = nullptr;
	mFormat = VK_FORMAT_UNDEFINED;
	mMemory = nullptr;
	mImageSize = 0;
	mWidth = mHeight = 0;
	mBpp = 0;
}

VulkanImage2D::~VulkanImage2D()
{
	vkDestroyImage(mHandle->getLogicalDevice(), mImage, nullptr);
}

void VulkanImage2D::construct(const uint32_t width, const uint32_t height, const uint8_t bpp, const ImageFormat& format, const ImageDataType& type, const ImageType& imageType)
{
	mImageSize = static_cast<VkDeviceSize>(width) * static_cast<VkDeviceSize>(height) * static_cast<VkDeviceSize>(bpp);
	mWidth = width;
	mHeight = height;
	mBpp = bpp;

	mImageFormat = format;
	mImageDataType = type;
	mImageType = imageType;

	mFormat = convertQgfxFormatToVulkan(format, type);
}

void VulkanImage2D::setData(const uint8_t* data, const uint32_t dataSize)
{
	if (data == nullptr)
	{
		QGFX_ASSERT_MSG(false, "Data is invalid");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(mHandle->getLogicalDevice(), mImage, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(mHandle->getPhysicalDevice(), memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	VkResult result = vkAllocateMemory(mHandle->getLogicalDevice(), &allocInfo, nullptr, &mMemory);
	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to allocated image memory!");

	vkBindImageMemory(mHandle->getLogicalDevice(), mImage, mMemory, 0);

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	createBuffer(mHandle->getLogicalDevice(), mHandle->getPhysicalDevice(), mImageSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* vulkanData;
	vkMapMemory(mHandle->getLogicalDevice(), stagingBufferMemory, 0, mImageSize, 0, &vulkanData);
	memcpy(vulkanData, data, static_cast<size_t>(mImageSize));
	vkUnmapMemory(mHandle->getLogicalDevice(), stagingBufferMemory);

	delete data;

	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = mWidth;
	imageInfo.extent.height = mHeight;
	imageInfo.extent.depth = 1;
	imageInfo.arrayLayers = 1;

	imageInfo.format = mFormat;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.flags = 0;

	result = vkCreateImage(mHandle->getLogicalDevice(), &imageInfo, nullptr, &mImage);

	QGFX_ASSERT_MSG(result == VK_SUCCESS, "Failed to create image");
}

void* VulkanImage2D::getImageHandle() const
{
	return mImage;
}

#endif // QGFX_VULKAN
