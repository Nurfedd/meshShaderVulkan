#pragma once
#include "Interface/surface.hpp"
#include "Interface/device.hpp"
#include "Interface/texture.hpp"
#include "Interface/fence.hpp"
#include "Interface/semaphore.hpp"
#include "Interface/command_buffer.hpp"
#include <vector>
namespace rhi {
	class VulkanSwapchain;
	struct SwapchainCreateInfo {
		PresentMode presentMode;
		uint32_t frameInFlight;
		SurfaceFormat surfaceFormat;
		Surface* surface;
	};
	struct SwapchainAcquireResult {
		bool success;
		uint32_t imageIndex;
	};
	class Swapchain {
	public :
		virtual ~Swapchain() = default;
		virtual void Create(Device* device, SwapchainCreateInfo swapchainCreateInfo);
		virtual SwapchainAcquireResult AcquireImage(Device* device,Fence* fence,Semaphore* semaphore) = 0;
		virtual void PreparePresentImage(CommandBuffer* commandBuffer) = 0;
		void AdvanceToNextImage() { currentFrame = (currentFrame + 1) % imageCount; }
		virtual void Recreate(Device* device) = 0;
		virtual void Destroy(Device* device) = 0;
		uint32_t GetImageCount() const { return imageCount; }
		uint32_t GetCurrentFrame() const { return currentFrame; }
		uint32_t GetImageIndex() const { return imageIndex; }
		Texture* GetTexture(uint32_t index) { return swapchainImageTextures[index]; }
		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;
		virtual VulkanSwapchain& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanSwapchain"); }
	protected :
		uint32_t imageCount;
		uint32_t currentFrame = 0;
		uint32_t imageIndex;

		PresentMode presentMode;
		SurfaceFormat surfaceFormat;
		Surface* surface;

		std::vector<Texture*> swapchainImageTextures;

		virtual void DestroyImages(Device* device) = 0;
		virtual void CreateImages(Device* device);
	};
}