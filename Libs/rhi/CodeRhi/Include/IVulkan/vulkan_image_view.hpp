#pragma once

#include "vulkan/vulkan.h"
#include "IVulkan/vulkan_image.hpp"
#include "IVulkan/vulkan_device.hpp"
namespace rhi {
	struct VulkanImageViewCreateInfo {
		VulkanImage* image;
		VkFormat format;
		VkImageViewType viewType;
		uint32_t baseMip = 0;
		uint32_t baseLayer = 0;
		uint32_t mipCount;
		uint32_t layerCount;
	};
	class VulkanImageView {
	public :
		void Create(VulkanDevice& vulkanDevice, VulkanImageViewCreateInfo viewCreateInfo);
		void Create(VulkanDevice& vulkanDevice,VulkanImage image,VkImageViewType viewType);
		void Destroy(VulkanDevice& device);
		VkImageViewType GetViewType() const { return viewType; }
		VkImageView GetVkImageView() const { return imageView; }
	private :
		VkImageAspectFlags GetImageAspectFromViewType(VkImageUsageFlags imageUsage);
		VkImageViewType viewType;
		VkImageView imageView;
		
	};
}