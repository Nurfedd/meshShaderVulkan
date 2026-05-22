#include "IVulkan/vulkan_pipeline.hpp"
#include "Volk/volk.h"
namespace rhi {

	void VulkanPipeline::PushConstantVk(VulkanCommandBuffer commandBuffer, uint32_t offset, size_t size, void* data, VkShaderStageFlags shaderStage) {
		vkCmdPushConstants(commandBuffer.commandBuffer, pipelineLayout, shaderStage, offset, size, data);
	}
	void VulkanPipeline::Bind(VulkanCommandBuffer cmd) {
		vkCmdBindPipeline(cmd.commandBuffer, GetBindPoint(), pipeline);
	}

	void VulkanPipeline::BindDescriptorSetsVk(VulkanCommandBuffer commandBuffer, VulkanDescriptorSet* sets, uint32_t setCount, uint32_t firstSet) {
		std::vector<VkDescriptorSet> vkSets(setCount);
		for (uint32_t i = 0; i < setCount; i++) {
			vkSets[i] = sets[i].GetSet();
		}
		vkCmdBindDescriptorSets(commandBuffer.commandBuffer, GetBindPoint(), pipelineLayout, firstSet, setCount, vkSets.data(), 0, nullptr);
	}
	
	void VulkanPipeline::Destroy(VkDevice device) {
		if (pipeline)
			vkDestroyPipeline(device, pipeline, nullptr);
		if (pipelineLayout)
			vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	}
}