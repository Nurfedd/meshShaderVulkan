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

	bool VulkanFence::IsReadyVk(VulkanDevice& vulkanDevice) {
		return vkGetFenceStatus(vulkanDevice, fence) == VK_SUCCESS;
	}
	
	bool VulkanFence::IsReady(Device* device) {
		return IsReadyVk(device->API_VULKAN());
	}
	void VulkanFence::Create(Device* device,bool initialReady) {
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		CreateVk(vulkanDevice, initialReady ? VK_FENCE_CREATE_SIGNALED_BIT : 0);
	}
	
	void VulkanFence::Destroy(Device* device) {
		DestroyVk(device->API_VULKAN());
	}

	void VulkanFence::DestroyVk(VulkanDevice& vulkanDevice) {
		vkDestroyFence(vulkanDevice, fence, nullptr);
	}
}