#pragma once
#include "Interface/physical_device.hpp"
#include "vulkan/vulkan.h"
#include <unordered_map>
const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};
namespace rhi {
	class VulkanPhysicalDevice : public PhysicalDevice {
	public :
		void Load(VkPhysicalDevice device);
		bool SupportSurface(Surface* surface) override;
		bool SupportExtensions();
		VkPhysicalDeviceProperties GetLimits();
		std::unordered_map<QueueType,uint32_t> GetFamilyQueueIndices(Surface* surface);
		VkPhysicalDevice GetVulkanPhysicalDevice() { return device; }
		VulkanPhysicalDevice& API_VULKAN() override { return *(this); }
	private :
		std::vector<VkQueueFamilyProperties> queueFamilies;
		void LoadInfo() override;
		VkPhysicalDevice device;
	};
}