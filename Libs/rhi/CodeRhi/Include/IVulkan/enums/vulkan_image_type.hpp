#pragma once
#include "enums/image_type.hpp"
#include "vulkan/vulkan.h"
static const VkImageViewType gImageViewToVk[] = {
	VK_IMAGE_VIEW_TYPE_2D,
	VK_IMAGE_VIEW_TYPE_CUBE,
};

namespace rhi {
	VkImageViewType ToVulkanViewType(ViewType viewType);
	ViewType FromVulkanViewType(VkImageViewType vkViewType);
	VkImageUsageFlags ToVulkanImageUsage(ImageUsage imageUsage);
	VkSamplerAddressMode ToVulkanAdressMode(SamplerAdressMode samplerAdress);
	VkFilter ToVulkanFilter(SamplerFilter samplerFilter);
}