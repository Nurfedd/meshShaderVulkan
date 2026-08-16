#pragma once
#include "Interface/swapchain.hpp"
#include "vulkan/vulkan.h"
#include "vulkan_queue_ownership.hpp"

namespace rhi {
	class VulkanSwapchain : public Swapchain {
	public :
		void Create(Device* device, SwapchainCreateInfo swapchainCreateInfo) override;
		SwapchainAcquireResult AcquireImage(Device* device, Fence* fence, Semaphore* semaphore) override;
		void PreparePresentImage(CommandBuffer* commandBuffer) override;
		void Recreate(Device* device) override;
		void Destroy(Device* device) override;
		VkSwapchainKHR GetSwapchain() { return swapchain; }
		VulkanSwapchain& API_VULKAN() override { return *(this); }
		uint32_t GetWidth() override { return swapchainExtent.width; }
		uint32_t GetHeight() override { return swapchainExtent.height; }
	private :
		
		VkSwapchainKHR swapchain = VK_NULL_HANDLE;
		VkExtent2D swapchainExtent;
		void DestroyImages(Device* device) override;
		void CreateImages(Device* device) override;
	};
}