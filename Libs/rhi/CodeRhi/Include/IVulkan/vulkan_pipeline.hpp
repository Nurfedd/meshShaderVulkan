#pragma once
#include "vulkan/vulkan.h"
#include "IVulkan/vulkan_command_buffer.hpp"
#include "IVulkan/vulkan_descriptor_set.hpp"
namespace rhi {
	class VulkanPipeline {
	public :
		virtual VkPipelineBindPoint GetBindPoint() const = 0;
		VkPipelineLayout GetPipelineLayout() const { return pipelineLayout; }
		void PushConstantVk(VulkanCommandBuffer commandBuffer,uint32_t offset,size_t size,void* data,VkShaderStageFlags shaderStage);
		void BindDescriptorSetsVk(VulkanCommandBuffer commandBuffer, VulkanDescriptorSet* sets, uint32_t setCount, uint32_t firstSet);
		virtual void Bind(VulkanCommandBuffer commandBuffer);
		virtual void Destroy(VkDevice device);
	protected :
		VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
		VkPipeline pipeline = VK_NULL_HANDLE;
	};
}