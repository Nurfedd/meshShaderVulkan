#pragma once
#include "Interface/descriptor_set.hpp"
#include "vulkan/vulkan.h"
#include "IVulkan/vulkan_buffer.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/vulkan_texture.hpp"
#include "IVulkan/vulkan_sampler.hpp"
namespace rhi {
	struct BufferUpdateInfoVk {
		VulkanBuffer** buffers;
		uint32_t count = 1;
		uint32_t dstArrayElement = 0;
	};
	class VulkanDescriptorSet : public DescriptorSet {
	public :
		void UpdateTextureVk(VulkanDevice& vulkanDevice, uint32_t binding, VulkanTexture& vulkanTexture, VulkanSampler& vulkanSampler, uint32_t dstArrayElement);
		void UpdateBufferVk(VulkanDevice& vulkanDevice, uint32_t binding, BufferUpdateInfoVk updatInfoVk,VkDescriptorType descriptorType);
		void UpdateTexture(Device* device, uint32_t binding, Texture* vulkanTexture, Sampler* vulkanSampler, uint32_t dstArrayElement) override;
		void UpdateBuffer(Device* device, uint32_t binding, BufferUpdateInfo bufferUpdateInfo,DescriptorType descriptorType) override;
		void SetVkSet(VkDescriptorSet set) { descriptorSet = set; }
		VulkanDescriptorSet& API_VULKAN() override { return *(this); };
		VkDescriptorSet GetSet() const { return descriptorSet; }
	private :
		VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
	};
}