#pragma once
#include "Interface/device_queue.hpp"
#include "IVulkan/vulkan_swapchain.hpp"
#include "Volk/volk.h"
#include <vector>
namespace rhi {
	class VulkanDevice;
	class VulkanFence;
	class VulkanSemaphore;
	class VulkanSwapchain;

	struct SemaphoreSubmitInfoVk {
		VulkanSemaphore* semaphores;
		uint32_t semaphoreCount = 0;
		VkPipelineStageFlags2* stageFlags;
		uint64_t* timelineValues;
	};

	struct VulkanSubmitInfo {
		SemaphoreSubmitInfoVk waitSemaphoreInfo;
		SemaphoreSubmitInfoVk signalSemaphoreInfo;
	};

	struct VulkanPresentInfo {
		VulkanSemaphore* waitSemaphores;
		uint32_t waitSemaphoreCount;
		VulkanSwapchain swapchain;
		
	};

	class VulkanDeviceQueue : public DeviceQueue {
	public :
		VulkanDeviceQueue(VkDevice vulkanDevice, uint32_t familyIndex);
		void SubmitVk(VulkanCommandBuffer* commandBuffer, uint32_t commandBufferCount, VulkanSubmitInfo submitInfo,VulkanFence* fence);
		void SubmitVk(VulkanCommandBuffer* commandBuffer, uint32_t commandBufferCount, VulkanFence* fence);
		bool PresentVk(VulkanPresentInfo vulkanPresentInfo);
		void Submit(CommandBuffer** commandsBuffer, uint32_t commandBufferCount, SubmitInfo submitInfo, Fence* fence) override;
		void Submit(CommandBuffer** commandsBuffer, uint32_t commandBufferCount, Fence* fence) override;
		
		bool Present(PresentInfo presentInfo) override;
		VkQueue GetVkQueue() const { return queue; }
		uint32_t GetFamilyIndex() const { return familyIndex; }
		VulkanDeviceQueue& API_VULKAN() { return *(this); }
	private :
		std::vector<VkSemaphoreSubmitInfo> GetSubmitInfo(SemaphoreSubmitInfoVk submitInfo);
		VkQueue queue;
		uint32_t familyIndex;
	};
}