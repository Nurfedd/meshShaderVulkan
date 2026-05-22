#include "IVulkan/vulkan_fence.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "Volk/volk.h"

namespace rhi {
	void VulkanFence::CreateVk(VulkanDevice& vulkanDevice, VkFenceCreateFlags flags) {
		VkFenceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		createInfo.flags = flags;

		vkCreateFence(vulkanDevice, &createInfo, nullptr, &fence);
	}

	void VulkanFence::Create(Device* device) {
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		CreateVk(vulkanDevice, VK_FENCE_CREATE_SIGNALED_BIT);
	}
	
	void VulkanFence::Destroy(Device* device) {
		DestroyVk(device->API_VULKAN());
	}

	void VulkanFence::DestroyVk(VulkanDevice& vulkanDevice) {
		vkDestroyFence(vulkanDevice, fence, nullptr);
	}
}