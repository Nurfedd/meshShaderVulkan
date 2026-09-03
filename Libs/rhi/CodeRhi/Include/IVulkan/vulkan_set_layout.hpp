#pragma once
#include "Interface/set_layout.hpp"
#include "vulkan/vulkan.h"
namespace rhi {
	class VulkanSetLayout : public SetLayout {
	public :
		void Create(Device* device, SetInfo* setInfos, uint32_t setInfoCount) override;
		void Destroy(Device* device) override;
		VkDescriptorSetLayout GetLayout() const { return descriptorSetLayout; }
		VulkanSetLayout& API_VULKAN() override { return *(this); }
	private :
		VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
	};
}