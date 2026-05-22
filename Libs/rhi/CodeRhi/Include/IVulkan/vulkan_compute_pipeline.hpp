#pragma once
#include "Interface/compute_pipeline.hpp"
#include "IVulkan/vulkan_pipeline.hpp"

namespace rhi {
	class VulkanComputePipeline : public ComputePipeline, public VulkanPipeline {
	public:
		VkPipelineBindPoint GetBindPoint() const override { return VK_PIPELINE_BIND_POINT_COMPUTE; }
		void Create(Device* device, ComputePipelineCreateInfo computePipelineCreateInfo) override;
		void PushConstant(CommandBuffer* commandBuffer, uint32_t offset, size_t size, void* data, ShaderStagesFlags shaderStages) override;
		void BindDescriptorSet(CommandBuffer* commandBuffer, DescriptorSet** sets, uint32_t setCount, uint32_t firstSet) override;
		void Bind(CommandBuffer* commandBuffer) override;
		void Destroy(Device* device) override;
		VulkanComputePipeline& API_VULKAN() override { return *(this); }
	
	};
}