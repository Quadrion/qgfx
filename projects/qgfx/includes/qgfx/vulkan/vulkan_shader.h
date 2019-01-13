#ifndef vulkan_shader_h__
#define vulkan_shader_h__

#include <string>

#include <vulkan/vulkan.h>
#include "qgfx/context_handle.h"

class VulkanShader
{
	public:
		explicit VulkanShader(ContextHandle* handle, const std::string& aSource);
		~VulkanShader();

	private:
		VkShaderModule mModule;
};

#endif // vulkan_shader_h__