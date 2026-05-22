#pragma once
#include "Interface/device.hpp"
#include "Interface/surface.hpp"
#include "IVulkan/vulkan_physical_device.hpp"
#include "IVulkan/vk_mem_alloc.h"
#include "IVulkan/vulkan_fence.hpp"
namespace rhi {
	class VulkanDevice : public Device {
	public:
		void Create(Instance* instance,PhysicalDevice* physicalDevice, Surface* surface) override;
		void WaitIdle() override;
		void WaitForFencesVk(VulkanFence* fences, uint32_t count, bool waitForAll, uint64_t timeout = UINT64_MAX);
		void WaitForFences(Fence** fences, uint32_t count,bool waitForAll, uint64_t timeout = UINT64_MAX) override;
		void ResetFencesVk(VulkanFence* fences, uint32_t count);
		void ResetFences(Fence** fences, uint32_t count) override;
		void Destroy() override;
		VulkanDevice& API_VULKAN() { return *(this); }

		VkPhysicalDevice GetVkPhysicalDevice() const { return physicalDevice->API_VULKAN().GetVulkanPhysicalDevice(); }
		VkDevice GetVkDevice() const { return device; }
		VmaAllocator GetAllocator() const { return allocator; }
		operator VkDevice() { return device; }
	private:
		void InitAllocator(VkInstance instance);
		VmaAllocator allocator;
		VkDevice device;
	};
}