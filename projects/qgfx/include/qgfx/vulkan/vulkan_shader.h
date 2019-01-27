#ifndef vulkan_shader_h__
#define vulkan_shader_h__

#include <qtl/string.h>
#include <qtl/vector.h>

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
		qtl::vector<void*> getStages() const override;

	private:
		VkShaderModule mVertexModule;
		VkShaderModule mFragmentModule;
		VkShaderModule mGeometryModule;
		VkShaderModule mTesselationControlModule;
		VkShaderModule mTesselationEvaluationModule;

		qtl::vector<VkPipelineShaderStageCreateInfo> mShaderStages;

		ContextHandle* mHandle;
};

#endif // vulkan_shader_h__