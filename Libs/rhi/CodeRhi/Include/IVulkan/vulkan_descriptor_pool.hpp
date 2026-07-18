#pragma once
#include "vulkan/vulkan.h"
#include "Interface/descriptor_pool.hpp"
#include "Interface/descriptor_set.hpp"
#include "IVulkan/vulkan_descriptor_set.hpp"
#include "IVulkan/vulkan_set_layout.hpp"
namespace rhi {
	class VulkanDescriptorPool : public DescriptorPool {
	public :
		void CreateVk(VulkanDevice& vulkanDevice, VkDescriptorPoolSize* poolSizes, uint32_t poolSizeCount, uint32_t maxSets, VkDescriptorPoolCreateFlags flags);
		void CreatePoolForImGui(VulkanDevice& vulkanDevice);
		void AllocateSetsVk(VulkanDevice& vulkanDevice, VulkanDescriptorSet** sets,VulkanSetLayout* layouts,uint32_t setCount,uint32_t* variableCounts);
		void AllocateSets(Device* device, DescriptorSet** sets, SetLayout** layouts, uint32_t setCount, uint32_t* variableCounts) override;
		void Reset(Device* device) override;
		void Create(Device* device, SetLayout** layouts, uint32_t layoutCount, uint32_t maxSets) override;
		void DestroyVk(VulkanDevice& vulkanDevice);
		void Destroy(Device* device) override;
		VkDescriptorPool GetPool() const { return descriptorPool; }
		VulkanDescriptorPool& API_VULKAN() override { return *(this); }
	private :
		VkDescriptorPool descriptorPool;
	};
}