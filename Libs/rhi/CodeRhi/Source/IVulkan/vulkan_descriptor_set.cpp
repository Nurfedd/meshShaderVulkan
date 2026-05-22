#include "IVulkan/vulkan_descriptor_set.hpp"
#include "IVulkan/enums/vulkan_input_format.hpp"
namespace rhi {

	void VulkanDescriptorSet::UpdateTextureVk(VulkanDevice& vulkanDevice, uint32_t binding, VulkanTexture& vulkanTexture, VulkanSampler& vulkanSampler, uint32_t dstArrayElement) {
		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageView = vulkanTexture.GetImageView()->GetVkImageView();
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.sampler = vulkanSampler.GetSampler();
		
		VkWriteDescriptorSet writeDescriptorSetInfo{};
		writeDescriptorSetInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSetInfo.dstBinding = binding;
		writeDescriptorSetInfo.descriptorCount = 1;
		writeDescriptorSetInfo.pImageInfo = &imageInfo;
		writeDescriptorSetInfo.dstArrayElement = dstArrayElement;
		writeDescriptorSetInfo.dstSet = descriptorSet;
		writeDescriptorSetInfo.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

		vkUpdateDescriptorSets(vulkanDevice, 1, &writeDescriptorSetInfo, 0, nullptr);
	}
	void VulkanDescriptorSet::UpdateBufferVk(VulkanDevice& vulkanDevice, uint32_t binding, BufferUpdateInfoVk updatInfoVk, VkDescriptorType descriptorType) {
		std::vector< VkDescriptorBufferInfo> bufferInfos(updatInfoVk.count);

		for (uint32_t i = 0; i < updatInfoVk.count; i++) {
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = updatInfoVk.buffers[i]->GetBuffer();
			bufferInfo.offset = 0;
			bufferInfo.range = VK_WHOLE_SIZE;
			bufferInfos[i] = bufferInfo;
		}
		
		VkWriteDescriptorSet writeDescriptorSetInfo{};
		writeDescriptorSetInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSetInfo.dstBinding = binding;
		writeDescriptorSetInfo.descriptorCount = updatInfoVk.count;
		writeDescriptorSetInfo.dstArrayElement = updatInfoVk.dstArrayElement;
		writeDescriptorSetInfo.dstSet = descriptorSet;
		writeDescriptorSetInfo.pBufferInfo = bufferInfos.data();
		writeDescriptorSetInfo.descriptorType = descriptorType;

		vkUpdateDescriptorSets(vulkanDevice, 1, &writeDescriptorSetInfo, 0, nullptr);
	}
	void VulkanDescriptorSet::UpdateBuffer(Device* device, uint32_t binding, BufferUpdateInfo bufferUpdateInfo, DescriptorType descriptorType) {
		BufferUpdateInfoVk updateInfoBufferVk;
		updateInfoBufferVk.count = bufferUpdateInfo.count;
		updateInfoBufferVk.dstArrayElement = bufferUpdateInfo.dstArrayElement;

		std::vector<VulkanBuffer*> vulkanBuffers(bufferUpdateInfo.count);
		for (uint32_t i = 0; i < vulkanBuffers.size(); i++) {
			vulkanBuffers[i] = &bufferUpdateInfo.buffers[i]->API_VULKAN();
		}

		updateInfoBufferVk.buffers = vulkanBuffers.data();

		UpdateBufferVk(device->API_VULKAN(), binding, updateInfoBufferVk, ToVulkanDecriptorType(descriptorType));
	}

	void VulkanDescriptorSet::UpdateTexture(Device* device, uint32_t binding, Texture* texture, Sampler* sampler, uint32_t dstArrayElement) {
		UpdateTextureVk(device->API_VULKAN(), binding, texture->API_VULKAN(), sampler->API_VULKAN(), dstArrayElement);
	}
}