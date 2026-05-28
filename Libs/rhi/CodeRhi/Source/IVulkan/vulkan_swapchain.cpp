#include "IVulkan/vulkan_swapchain.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/vulkan_surface.hpp"
#include "IVulkan/vulkan_texture.hpp"
#include "IVulkan/vulkan_device_queue.hpp"
#include "IVulkan/enums/vulkan_format.hpp"
#include "IVulkan/vulkan_fence.hpp"
#include "IVulkan/vulkan_semaphore.hpp"
#include "IVulkan/vulkan_command_buffer.hpp"
#include "Volk/volk.h"
#include <exception>

namespace rhi {
	void VulkanSwapchain::Create(Device* device, SwapchainCreateInfo swapchainCreateInfo) {
		Swapchain::Create(device, swapchainCreateInfo);
		if (surface == nullptr) {
			throw std::exception("Surface is null for swapchain creation");
			return;
		}  

		VulkanSurface& vulkanSurface = surface->API_VULKAN();
		VulkanDevice& vulkanDevice = device->API_VULKAN();

		VkSurfaceCapabilitiesKHR surfaceCapabilites;
		vulkanSurface.GetCapabilities(vulkanDevice.GetVkPhysicalDevice(), &surfaceCapabilites);

		DeviceQueue* graphicQueue = device->GetQueue(GRAPHIC_QUEUE);
		DeviceQueue* presentQueue = device->GetQueue(PRESENT_QUEUE);

		if (graphicQueue == nullptr || presentQueue == nullptr) {
			throw std::exception("Cannot create a swapchain without present queue and graphic queue");
			return;
		}
		VulkanDeviceQueue& vulkanGraphicQueue = graphicQueue->API_VULKAN();
		VulkanDeviceQueue& vulkanPresentQueue = presentQueue->API_VULKAN();

		queueOwnership.AddFamily(vulkanGraphicQueue.GetFamilyIndex());
		queueOwnership.AddFamily(vulkanPresentQueue.GetFamilyIndex());

		if (!surface->SupportPresentMode(presentMode) || !surface->SupportSurfaceFormat(surfaceFormat)) {
			presentMode = PresentMode::FIFO_MODE; // always supported
			surfaceFormat = SurfaceFormat(RGBA8_SRGB, SRGB_NON_LINEAR); // always supported
		}
		imageCount = std::max(uint32_t(1), swapchainCreateInfo.frameInFlight);
		if (surfaceCapabilites.maxImageCount != 0) {
			imageCount = std::min(imageCount, surfaceCapabilites.maxImageCount);
		}
		Recreate(device);
	}

	void VulkanSwapchain::Recreate(Device* device) {
		DestroyImages(device);
		VulkanSurface& vulkanSurface = surface->API_VULKAN();
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		VkSurfaceCapabilitiesKHR capabilities;
		vulkanSurface.GetCapabilities(vulkanDevice.GetVkPhysicalDevice(), &capabilities);
		swapchainExtent = capabilities.currentExtent;
		VkSwapchainKHR oldSwaphain = swapchain;

		std::unordered_set<uint32_t> familyIndicesSet = queueOwnership.GetIndices();
		std::vector<uint32_t> familyIndices(familyIndicesSet.begin(), familyIndicesSet.end());

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = vulkanSurface.GetSurface();
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = ToVulkanFormat(surfaceFormat.format);
		createInfo.imageColorSpace = ToVulkanColorSpace(surfaceFormat.colorSpace);
		createInfo.imageExtent = capabilities.currentExtent;
		createInfo.preTransform = capabilities.currentTransform;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = ToVulkanPresentMode(presentMode);
		createInfo.clipped = VK_TRUE;
		createInfo.imageSharingMode = queueOwnership.GetSharingMode();
		createInfo.pQueueFamilyIndices = familyIndices.data();
		createInfo.queueFamilyIndexCount = familyIndices.size();
		
		createInfo.oldSwapchain = swapchain;

		if (vkCreateSwapchainKHR(vulkanDevice, &createInfo, nullptr, &swapchain) != VK_SUCCESS) {
			throw std::exception("swapchain creation failed");
		}

		if (oldSwaphain) {
			vkDestroySwapchainKHR(vulkanDevice, oldSwaphain, nullptr);
		}
		CreateImages(device);
	}

	SwapchainAcquireResult VulkanSwapchain::AcquireImage(Device* device, Fence* fence, Semaphore* semaphore) {
		SwapchainAcquireResult acquireResult;
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		
		VulkanSemaphore& vulkanSemaphore = semaphore->API_VULKAN();

		if ( semaphore && !semaphore->IsBinary()) {
			throw std::exception("Semaphore must be a binary semaphore for acquire image");
			return acquireResult;
		}
		VkResult result = vkAcquireNextImageKHR(
			vulkanDevice,
			swapchain,
			UINT64_MAX,
			(semaphore == nullptr) ? VK_NULL_HANDLE : semaphore->API_VULKAN().GetSemaphore(),
			(fence == nullptr) ? VK_NULL_HANDLE : fence->API_VULKAN().GetFence(),
			&imageIndex
		);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
			acquireResult.success = false;
		}
		else {
			acquireResult.success = true;
			acquireResult.imageIndex = imageIndex;
		}
		return acquireResult;
	}

	void VulkanSwapchain::PreparePresentImage(CommandBuffer* commandBuffer) {
		VulkanCommandBuffer& vulkanCommandBuffer = commandBuffer->API_VULKAN();
		VulkanTexture& vulkanPresentTexture = swapchainImageTextures[imageIndex]->API_VULKAN();
		vulkanPresentTexture.GetImage()->TransitionImage(
			vulkanCommandBuffer,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT,
			0,
			VK_IMAGE_ASPECT_COLOR_BIT
		);
	}
	 
	void VulkanSwapchain::CreateImages(Device* device) {
		Swapchain::CreateImages(device);
		VulkanDevice& vulkanDevice = device->API_VULKAN();

		uint32_t count;
		vkGetSwapchainImagesKHR(vulkanDevice, swapchain, &count, nullptr);
		std::vector<VkImage> vkImages(count);
		vkGetSwapchainImagesKHR(vulkanDevice, swapchain, &count, vkImages.data());

		for (uint32_t i = 0; i < count; i++) {
			VulkanTexture* vulkanTexture = new VulkanTexture;
			VulkanImage image;
			image.CreateFromVkImage(vkImages[i], ToVulkanFormat(surfaceFormat.format));
			image.SetStage(VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,0); 
			VulkanImageViewCreateInfo imageViewCreateInfo;
			imageViewCreateInfo.image = &image;
			imageViewCreateInfo.format = ToVulkanFormat(surfaceFormat.format);
			imageViewCreateInfo.layerCount = 1;
			imageViewCreateInfo.mipCount = 1;
			imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

			VulkanImageView imageView;
			imageView.Create(vulkanDevice, imageViewCreateInfo);

			vulkanTexture->CreateVk(image, imageView,GetWidth(),GetHeight());
			swapchainImageTextures[i] = vulkanTexture;
		}
	}
	void VulkanSwapchain::DestroyImages(Device* device) {
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		for (uint32_t i = 0; i < swapchainImageTextures.size(); i++) {
			Texture* texture = swapchainImageTextures[i];
			VulkanTexture& vulkanTexture = texture->API_VULKAN();
			vulkanTexture.GetImageView()->Destroy(vulkanDevice);
			delete texture;
		}
		swapchainImageTextures.clear();
	}
	void VulkanSwapchain::Destroy(Device* device) {
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		DestroyImages(device);
		if (swapchain) {
			vkDestroySwapchainKHR(vulkanDevice, swapchain, nullptr);
		}
	}
}