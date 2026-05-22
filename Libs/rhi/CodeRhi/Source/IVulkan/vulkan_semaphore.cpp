#include "IVulkan/vulkan_semaphore.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "Volk/volk.h"
namespace rhi {
	
	void VulkanSemaphore::CreateBinary(Device* device) {
		isBinary = true;
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		VkSemaphoreCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		vkCreateSemaphore(vulkanDevice, &createInfo, nullptr, &semaphore);
	}

	void VulkanSemaphore::CreateTimeline(Device* device, uint64_t initialValue) {
		isBinary = false;
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		VkSemaphoreTypeCreateInfo typeInfo{};
		typeInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO;
		typeInfo.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE;
		typeInfo.initialValue = initialValue;

		VkSemaphoreCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		createInfo.pNext = &typeInfo;

		vkCreateSemaphore(vulkanDevice, &createInfo, nullptr, &semaphore);
	}

	uint64_t VulkanSemaphore::GetTimelineValue(Device* device) {
		if (isBinary)
			return 0;
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		uint64_t value;
		vkGetSemaphoreCounterValue(vulkanDevice, semaphore, &value);
		return value;
	}

	void VulkanSemaphore::Destroy(Device* device) {
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		vkDestroySemaphore(vulkanDevice, semaphore, nullptr);
	}
}