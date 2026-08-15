#include "IVulkan/vulkan_command_pool.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/vulkan_device_queue.hpp"
#include "IVulkan/vulkan_command_buffer.hpp"
#include "Volk/volk.h"
#include <exception>

namespace rhi {
	void VulkanCommandPool::Create(Device* device, DeviceQueue* _queue) {
		CommandPool::Create(device, _queue);
		VulkanDeviceQueue& vulkanQueue = queue->API_VULKAN();
		VulkanDevice& vulkanDevice = device->API_VULKAN();

		VkCommandPoolCreateInfo createInfo{};
		createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		createInfo.queueFamilyIndex = vulkanQueue.GetFamilyIndex();
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		if (vkCreateCommandPool(vulkanDevice, &createInfo, nullptr, &commandPool) != VK_SUCCESS)
			throw std::exception("Couldn't create command pool");
	}

	void VulkanCommandPool::AllocateCommandBufferVk(VulkanDevice& vulkanDevice, VkCommandBuffer* commandBuffers, uint32_t count, VkCommandBufferLevel level) {
		if (count == 0)
			return;
		
		VkCommandBufferAllocateInfo allocateInfo{};
		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.commandBufferCount = count;
		allocateInfo.level = level;
		allocateInfo.commandPool = commandPool;
		if (vkAllocateCommandBuffers(vulkanDevice, &allocateInfo, commandBuffers) != VK_SUCCESS) {
			throw std::exception("Couldn't allocate command buffers");
		}
	}
	void VulkanCommandPool::AllocateCommandBuffers(Device* device,CommandBuffer** commandBuffers, uint32_t count) {
		if (count == 0)
			return;
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		std::vector<VkCommandBuffer> vkCommandBuffers(count);
		
		AllocateCommandBufferVk(vulkanDevice, vkCommandBuffers.data(), count, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		for (uint32_t i = 0; i < count; i++) {
			VulkanCommandBuffer& vulkanCommandBuffer = commandBuffers[i]->API_VULKAN();
			vulkanCommandBuffer.commandBuffer = vkCommandBuffers[i];
		}
		
	}
	void VulkanCommandPool::Destroy(Device* device) {
		DestroyVk(device->API_VULKAN());
	}

	void VulkanCommandPool::DestroyVk(VulkanDevice& vulkanDevice) {
		if (commandPool)
			vkDestroyCommandPool(vulkanDevice, commandPool, nullptr);
	}
}