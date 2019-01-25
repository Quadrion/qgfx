#ifndef vulkan_shader_h__
#define vulkan_shader_h__

#include <string>

#include <vulkan/vulkan.h>

#include "qgfx/context_handle.h"
#include "qgfx/api/ishader.h"

class VulkanShader : public IShader
{
	public:
		explicit VulkanShader(ContextHandle* context);
		~VulkanShader();

		bool attachVertexShader(const std::string& source) override;
		bool attachFragmentShader(const std::string& source) override;
		bool attachGeometryShader(const std::string& source) override;
		bool attachTesselationControlShader(const std::string& source) override;
		bool attachTesselationEvaluationShader(const std::string& source) override;

		bool compile() override;

		bool bind() override;
		bool unbind() override;

	private:
		VkShaderModule mVertexModule;
		VkShaderModule mFragmentModule;
		VkShaderModule mGeometryModule;
		VkShaderModule mTesselationControlModule;
		VkShaderModule mTesselationEvaluationModule;

		std::vector<VkPipelineShaderStageCreateInfo> mShaderStages;
};

#endif // vulkan_shader_h__