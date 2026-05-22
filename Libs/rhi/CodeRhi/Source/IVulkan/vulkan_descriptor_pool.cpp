#include "IVulkan/vulkan_descriptor_pool.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/vulkan_set_layout.hpp"

#include "IVulkan/enums/vulkan_input_format.hpp"
#include "Volk/volk.h"
#include <unordered_map>
#include <exception>
#include <vector>
namespace rhi {

	void VulkanDescriptorPool::CreateVk(VulkanDevice& vulkanDevice, VkDescriptorPoolSize* poolSizes, uint32_t poolSizeCount, uint32_t maxSets, VkDescriptorPoolCreateFlags flags) {
		VkDescriptorPoolCreateInfo createInfo{};
		createInfo.flags = flags;
		createInfo.poolSizeCount = poolSizeCount;
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		createInfo.maxSets = maxSets;
		createInfo.pPoolSizes = poolSizes;
		if (vkCreateDescriptorPool(vulkanDevice, &createInfo, nullptr, &descriptorPool) != VK_SUCCESS)
			throw std::exception("failed to create descritpor pool");
	}

	void VulkanDescriptorPool::CreatePoolForImGui(VulkanDevice& vulkanDevice) {
		VkDescriptorPoolSize pool_sizes[] = {
	{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
	{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
	{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
	{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
	{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
	{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
	{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
	{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
	{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
	{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
	{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};
		CreateVk(vulkanDevice, pool_sizes, 11, 11 * 1000, VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT);
	}

	void VulkanDescriptorPool::AllocateSetsVk(VulkanDevice& vulkanDevice, VulkanDescriptorSet** sets,VulkanSetLayout* setLayouts, uint32_t setCount,uint32_t* variableCounts) {

		std::vector<VkDescriptorSetLayout> vkLayouts(setCount);
		std::vector<VkDescriptorSet> vkSets(setCount);

		for (uint32_t i = 0; i < setCount; i++) {
			vkLayouts[i] = setLayouts[i].GetLayout();
		}

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = setCount;
		allocInfo.pSetLayouts = vkLayouts.data();

		VkDescriptorSetVariableDescriptorCountAllocateInfo countInfo{};
		if (variableCounts != nullptr) {
			countInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO;
			countInfo.descriptorSetCount = setCount;
			countInfo.pDescriptorCounts = variableCounts;

			allocInfo.pNext = &countInfo;
		}

		if (vkAllocateDescriptorSets(vulkanDevice, &allocInfo, vkSets.data()) != VK_SUCCESS) {
			throw std::exception("Couldn't allocate descriptor sets !");
		}

		for (uint32_t i = 0; i < setCount; i++) {
			sets[i]->SetVkSet(vkSets[i]);
		}
	}

	void VulkanDescriptorPool::Create(Device* device, SetLayout** layouts, uint32_t count, uint32_t maxSets) {
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		std::unordered_map<VkDescriptorType, uint32_t> layoutInfos;

		for (uint32_t i = 0; i < count;i++) {
			VulkanSetLayout& vulkanSetLayout = layouts[i]->API_VULKAN();
			std::unordered_map<DescriptorType, uint32_t> info = vulkanSetLayout.GetLayoutInfo();
			for (auto& pair : info) {
				layoutInfos[ToVulkanDecriptorType(pair.first)] += pair.second;
			}
		}
		std::vector<VkDescriptorPoolSize> poolSizes;
		for (auto& pair : layoutInfos) {
			VkDescriptorPoolSize poolSizeInfo;
			poolSizeInfo.descriptorCount = pair.second;
			poolSizeInfo.type = pair.first;
			poolSizes.push_back(poolSizeInfo);
		}
		CreateVk(vulkanDevice, poolSizes.data(), poolSizes.size(), maxSets, VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT);
	}

	void VulkanDescriptorPool::AllocateSets(Device* device, DescriptorSet** sets, SetLayout** layouts, uint32_t setCount,uint32_t* variableCounts) {
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		std::vector<VulkanDescriptorSet*> vulkanDescriptorSets(setCount);
		std::vector<VulkanSetLayout> vulkanSetLayouts(setCount);

		for (uint32_t i = 0; i < setCount; i++) {
			vulkanDescriptorSets[i] = &sets[i]->API_VULKAN();
			vulkanSetLayouts[i] = layouts[i]->API_VULKAN();
		}

		AllocateSetsVk(vulkanDevice, vulkanDescriptorSets.data(),vulkanSetLayouts.data(), setCount,variableCounts);
	}
	void VulkanDescriptorPool::DestroyVk(VulkanDevice& vulkanDevice) {
		vkDestroyDescriptorPool(vulkanDevice, descriptorPool, nullptr);
	}
	
	void VulkanDescriptorPool::Destroy(Device* device) {
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		DestroyVk(vulkanDevice);
	}
}