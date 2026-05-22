#include "IVulkan/vulkan_compute_pipeline.hpp"
#include "IVulkan/vulkan_set_layout.hpp"
#include "IVulkan/vulkan_push_constant_range.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/vulkan_command_buffer.hpp"
#include "IVulkan/vulkan_shader.hpp"
#include "IVulkan/enums/vulkan_input_format.hpp"
#include "Volk/volk.h"

namespace rhi {
	void VulkanComputePipeline::Create(Device* device, ComputePipelineCreateInfo createInfo) {
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		std::vector<VkDescriptorSetLayout> layouts(createInfo.setLayoutCount);
		
		for (int i = 0; i < createInfo.setLayoutCount; i++) {
			VulkanSetLayout& vulkanSetLayout = createInfo.setLayouts[i]->API_VULKAN();
			layouts[i] = vulkanSetLayout.GetLayout();
		}

		std::vector<VkPushConstantRange> pushConstantRanges(createInfo.pushConstantCount);

		for (int i = 0; i < createInfo.pushConstantCount; i++) {
			VulkanPushConstantRange& vulkanPushConstantRange = createInfo.pushConstants[i]->API_VULKAN();
			pushConstantRanges[i] = vulkanPushConstantRange.GetRange();
		}

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = createInfo.setLayoutCount;
		pipelineLayoutInfo.pSetLayouts = layouts.data();
		pipelineLayoutInfo.pPushConstantRanges = pushConstantRanges.data();
		pipelineLayoutInfo.pushConstantRangeCount = createInfo.pushConstantCount;

		if (vkCreatePipelineLayout(vulkanDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!");
		}

		VkPipelineShaderStageCreateInfo shaderInfo{};
		shaderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderInfo.module = createInfo.computeShader->API_VULKAN().GetModule();
		shaderInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
		shaderInfo.pName = "main";

		VkComputePipelineCreateInfo computeCreateInfo{};
		computeCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		computeCreateInfo.layout = pipelineLayout;
		computeCreateInfo.flags = 0;
		computeCreateInfo.basePipelineHandle = nullptr;
		computeCreateInfo.stage = shaderInfo;

		if (vkCreateComputePipelines(vulkanDevice, nullptr, 1, &computeCreateInfo, nullptr, &pipeline) != VK_SUCCESS) {
			throw std::runtime_error("failed to create compute pipeline !");
		}
	}

	void VulkanComputePipeline::PushConstant(CommandBuffer* commandBuffer, uint32_t offset, size_t size, void* data, ShaderStagesFlags shaderStages) {
		PushConstantVk(commandBuffer->API_VULKAN(), offset, size, data, ToVulkanShaderStage(shaderStages));
	}

	void VulkanComputePipeline::BindDescriptorSet(CommandBuffer* commandBuffer, DescriptorSet** sets, uint32_t setCount, uint32_t firstSet) {
		std::vector<VulkanDescriptorSet> vulkanSets(setCount);
		for (uint32_t i = 0; i < setCount; i++) {
			vulkanSets[i] = sets[i]->API_VULKAN();
		}
		BindDescriptorSetsVk(commandBuffer->API_VULKAN(), vulkanSets.data(), setCount, firstSet);
	}
	
	void VulkanComputePipeline::Destroy(Device* device) {
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		VulkanPipeline::Destroy(vulkanDevice);
	}

	void VulkanComputePipeline::Bind(CommandBuffer* commandBuffer) {
		VulkanCommandBuffer& vulkanCommandBuffer = commandBuffer->API_VULKAN();
		VulkanPipeline::Bind(vulkanCommandBuffer);
	}
}