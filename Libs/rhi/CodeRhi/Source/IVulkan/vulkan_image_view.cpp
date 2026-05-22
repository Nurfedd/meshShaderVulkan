#include "IVulkan/vulkan_image_view.hpp"
#include "IVulkan/vulkan_image.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/enums/vulkan_format.hpp"
#include "IVulkan/enums/vulkan_image_type.hpp"
#include <exception>
#include "Volk/volk.h"
namespace rhi {
	void VulkanImageView::Create(VulkanDevice& vulkanDevice, VulkanImageViewCreateInfo viewCreateInfo) {
		viewType = viewCreateInfo.viewType;
		VkImageViewCreateInfo imageCreateInfo{};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageCreateInfo.image = viewCreateInfo.image->GetVkImage();
		imageCreateInfo.format = viewCreateInfo.format;

		imageCreateInfo.viewType = viewCreateInfo.viewType;
		imageCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		imageCreateInfo.subresourceRange.aspectMask = GetImageAspectFromViewType(viewCreateInfo.image->GetImageUsage());
		imageCreateInfo.subresourceRange.baseMipLevel = viewCreateInfo.baseMip;
		imageCreateInfo.subresourceRange.levelCount = viewCreateInfo.mipCount;
		imageCreateInfo.subresourceRange.baseArrayLayer = viewCreateInfo.baseLayer;
		imageCreateInfo.subresourceRange.layerCount = viewCreateInfo.layerCount;

		if (vkCreateImageView(vulkanDevice, &imageCreateInfo, nullptr, &imageView) != VK_SUCCESS) {
			throw std::exception("failed to create image views!");
		}
	}
	void VulkanImageView::Create(VulkanDevice& vulkanDevice, VulkanImage vulkanImage, VkImageViewType viewType) {
		
		VulkanImageViewCreateInfo createInfo;
		createInfo.image = &vulkanImage;
		createInfo.viewType = viewType;
		createInfo.baseMip = 0;
		createInfo.baseLayer = 0;
		createInfo.format = vulkanImage.GetFormat();
		createInfo.mipCount = vulkanImage.GetMipLevel();
		createInfo.layerCount = vulkanImage.GetArrayLayerCount();

		Create(vulkanDevice, createInfo);

	}

	VkImageAspectFlags VulkanImageView::GetImageAspectFromViewType(VkImageUsageFlags imageUsage) {
		if (imageUsage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
			return VK_IMAGE_ASPECT_DEPTH_BIT;
		return VK_IMAGE_ASPECT_COLOR_BIT;
	}
	void VulkanImageView::Destroy(VulkanDevice& vulkanDevice) {
		vkDestroyImageView(vulkanDevice, imageView, nullptr);
	}
}