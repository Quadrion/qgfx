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

		bool attachVertexShader(const qtl::string& source) override;
		bool attachFragmentShader(const qtl::string& source) override;
		bool attachGeometryShader(const qtl::string& source) override;
		bool attachTesselationControlShader(const qtl::string& source) override;
		bool attachTesselationEvaluationShader(const qtl::string& source) override;

		bool compile() override;

		bool bind() override;
		bool unbind() override;

		uint32_t getStageCount() const override;

	private:
		VkShaderModule mVertexModule;
		VkShaderModule mFragmentModule;
		VkShaderModule mGeometryModule;
		VkShaderModule mTesselationControlModule;
		VkShaderModule mTesselationEvaluationModule;

		std::vector<VkPipelineShaderStageCreateInfo> mShaderStages;
};

#endif // vulkan_shader_h__