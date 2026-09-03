#include "IVulkan/vulkan_set_layout.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/enums/vulkan_input_format.hpp"
#include "Volk/volk.h"
#include <exception>
namespace rhi {
	void VulkanSetLayout::Create(Device* device, SetInfo* setInfos, uint32_t setInfoCount) {
		SetLayout::Create(device, setInfos, setInfoCount);

		VulkanDevice& vulkanDevice = device->API_VULKAN();
		std::vector<VkDescriptorSetLayoutBinding> layoutsBindings(setInfoCount);
		std::vector<VkDescriptorBindingFlags> bindingsFlags(setInfoCount);

		for (uint32_t i = 0; i < setInfoCount; i++) {
			SetInfo info = setInfos[i];
			VkDescriptorSetLayoutBinding layoutBinding{};
			
			layoutBinding.binding = info.binding;
			layoutBinding.descriptorCount = info.count;
			layoutBinding.descriptorType = ToVulkanDecriptorType(info.descriptorType);
			layoutBinding.stageFlags = ToVulkanShaderStage(info.shaderStages);
			layoutBinding.pImmutableSamplers = nullptr;
			layoutsBindings[i] = layoutBinding;


			VkDescriptorBindingFlags bindingFlag =
				VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT |
				VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT;

			if (info.variable) {
				bindingFlag |= VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT;
			}
			bindingsFlags[i] = bindingFlag;
		}
		
		VkDescriptorSetLayoutBindingFlagsCreateInfo flagsInfo{};
		flagsInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
		flagsInfo.bindingCount = setInfoCount;
		flagsInfo.pBindingFlags = bindingsFlags.data();

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = layoutsBindings.size();
		layoutInfo.pBindings = layoutsBindings.data();
		layoutInfo.pNext = &flagsInfo;
		layoutInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;

		if (vkCreateDescriptorSetLayout(vulkanDevice, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
			throw std::exception("failed to create descriptor set layout!");
		}
	}

	void VulkanSetLayout::Destroy(Device* device) {
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		vkDestroyDescriptorSetLayout(vulkanDevice, descriptorSetLayout, nullptr);
	}
}