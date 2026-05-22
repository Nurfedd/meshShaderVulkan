#include "Interface/graphic_pipeline.hpp"
#include "IVulkan/vulkan_pipeline.hpp"

namespace rhi {
	class VulkanGraphicPipeline : public GraphicPipeline,public VulkanPipeline {
	public :
		VkPipelineBindPoint GetBindPoint() const override { return VK_PIPELINE_BIND_POINT_GRAPHICS; }
		
		void Create(Device* device, GraphicPipelineCreateInfo graphicPipelineCreateInfo) override;
		void PushConstant(CommandBuffer* commandBuffer, uint32_t offset, size_t size, void* data, ShaderStagesFlags shaderStages) override;
		void BindDescriptorSet(CommandBuffer* commandBuffer, DescriptorSet** sets, uint32_t setCount, uint32_t firstSet) override;
		void Bind(CommandBuffer* commandBuffer) override;
		void Destroy(Device* device) override;
		std::vector<VkFormat> GetFormats() { return usedColorFormats; }
		VkFormat GetDepthFormat() { return usedDepthFormat; }
		VulkanGraphicPipeline& API_VULKAN() override { return *(this); }
	private :
		std::vector<VkFormat> usedColorFormats;
		VkFormat usedDepthFormat;
		void CreateShaderStages(GraphicPipelineCreateInfo createInfo, std::vector<VkPipelineShaderStageCreateInfo>& result);
	};
}