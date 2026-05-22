#pragma once
#include "IVulkan/vulkan_device.hpp"
#include "vulkan/vulkan.h"
namespace rhi {
	class VulkanCommandBuffer;
	struct VulkanImageCreateInfo {
		uint32_t width;
		uint32_t height;
		VkImageUsageFlags usageFlags;
		uint32_t arrayLayers;
		uint32_t mipLevel;
		VkFormat format;
		
	};
	class VulkanImage {
	public :
		/*used with swapchain image vkGetSwapchainImagesKHR*/
		void CreateFromVkImage(VkImage image,VkFormat format);
		void Create(VulkanDevice& vulkanDevice, VulkanImageCreateInfo createInfo);
		void Destroy(VulkanDevice& vulkanDevice);
		void TransitionImage(VulkanCommandBuffer cmdBuffer,VkImageLayout newLayout,VkPipelineStageFlags2 dstStage,VkAccessFlags2 dstAccess);
		void TransitionImage(VulkanCommandBuffer cmdBuffer, VkImageLayout newLayout, VkPipelineStageFlags2 dstStage, VkAccessFlags2 dstAccess, VkImageAspectFlags aspectMask);
		void TransitionImage(VulkanCommandBuffer cmdBuffer, VkImageLayout newLayout, VkPipelineStageFlags2 dstStage, VkAccessFlags2 dstAccess, uint32_t baseMip,uint32_t mipCount);
		void TransitionImage(VulkanCommandBuffer cmdBuffer, VkImageLayout newLayout, VkPipelineStageFlags2 dstStage, VkAccessFlags2 dstAccess, uint32_t baseMip, uint32_t mipCount,VkImageAspectFlags aspectMask);
		void TransitionImage(VulkanCommandBuffer cmdBuffer, VkImageLayout oldLayout,VkImageLayout newLayout,VkPipelineStageFlags2 srcStage,VkPipelineStageFlags2 dstStage,VkAccessFlags2 srcAccess,VkAccessFlags2 dstAccess,uint32_t baseMip,uint32_t mipCount,VkImageAspectFlags aspectMask);
		
		void TransitionImage(VulkanCommandBuffer cmdBuffer, VkImageLayout newLayout, VkPipelineStageFlags2 dstStage, VkAccessFlags2 dstAccess,VkImageSubresourceRange subresrouceRange);
		void SetStage(VkPipelineStageFlags2 stage, VkAccessFlags2 access) { currentAccess = access; currentStage = stage; }
		VkImageLayout GetCurrentLayout() const { return currentLayout; }
		VkImage GetVkImage() const { return image; }
		uint32_t GetMipLevel() const { return mipLevel; }
		uint32_t GetArrayLayerCount() const { return arrayLayer; }
		VkFormat GetFormat() const { return imageFormat; }
		VkImageUsageFlags GetImageUsage() const { return usageFlags; }
	private :
		VkImage image = VK_NULL_HANDLE;
		VkImageUsageFlags usageFlags;
		VkFormat imageFormat;
		VkImageLayout currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		VkAccessFlags2 currentAccess = 0;
		VkPipelineStageFlags2 currentStage = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT;
		VmaAllocation allocation = nullptr;
		uint32_t mipLevel;
		uint32_t arrayLayer;
	};
}