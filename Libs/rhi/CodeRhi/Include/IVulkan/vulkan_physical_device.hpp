#pragma once
#include "Interface/physical_device.hpp"
#include "vulkan/vulkan.h"
#include <unordered_map>
const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};
namespace rhi {
	struct QueueInfo {
		QueueInfo() = default;
		QueueInfo(QueueType queueType, uint32_t familyIndice) : queueType(queueType), familyIndice(familyIndice),isValid(true) {}

		QueueType queueType;
		uint32_t familyIndice;

		bool IsValid() { return isValid; }
	private :
		bool isValid = false;
	};

	struct PhysicalDeviceQueueInfos {
		
		std::unordered_map<uint32_t, uint32_t> GetQueueFamilyIndiceAndCount();
		QueueInfo graphicQueue;
		QueueInfo computeQueue;
		QueueInfo transferQueueCpuToGpu;
		QueueInfo transferQueueGpuToCpu;
	
	};
	class VulkanPhysicalDevice : public PhysicalDevice {
	public :
		void Load(VkPhysicalDevice device);
		bool SupportSurface(Surface* surface) override;
		bool FamilySupportPresent(uint32_t familyIndex,const VulkanSurface& surface);
		bool SupportExtensions();
		VkPhysicalDeviceProperties GetLimits();
		PhysicalDeviceQueueInfos GetFamilyQueueIndices(Surface* surface);
		VkPhysicalDevice GetVulkanPhysicalDevice() { return device; }
		VulkanPhysicalDevice& API_VULKAN() override { return *(this); }

		
	private :
		VkQueueFlags RhiQueueTypeToVulkanQueueFlags(QueueType queueType);
		std::vector<VkQueueFamilyProperties> queueFamilies;
		void LoadInfo() override;
		VkPhysicalDevice device;
	};
}