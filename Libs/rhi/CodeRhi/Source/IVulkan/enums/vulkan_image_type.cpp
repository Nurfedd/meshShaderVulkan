#include "IVulkan/enums/vulkan_image_type.hpp"

namespace rhi {
	VkImageViewType ToVulkanViewType(ViewType viewType) {
		return gImageViewToVk[static_cast<uint32_t>(viewType)];
	}
	ViewType FromVulkanViewType(VkImageViewType vkViewType) {
		switch (vkViewType)
		{
		case VK_IMAGE_VIEW_TYPE_2D: return VIEW_2D;
		case VK_IMAGE_VIEW_TYPE_CUBE: return VIEW_CUBE;
		}
	}
    VkImageUsageFlags ToVulkanImageUsage(ImageUsage usage)
    {
        VkImageUsageFlags flags = 0;

        if (usage & ImageUsage::TRANSFER_SRC_USAGE)
            flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

        if (usage & ImageUsage::TRANSFER_DST_USAGE)
            flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;

        if (usage & ImageUsage::SAMPLED_USAGE)
            flags |= VK_IMAGE_USAGE_SAMPLED_BIT;

        if (usage & ImageUsage::STORAGE_USAGE)
            flags |= VK_IMAGE_USAGE_STORAGE_BIT;

        if (usage & ImageUsage::COLOR_ATTACHMENT_USAGE)
            flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        if (usage & ImageUsage::DEPTH_STENCIL_ATTACHMENT_USAGE)
            flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

        if (usage & ImageUsage::INPUT_ATTACHMENT_USAGE)
            flags |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;

        return flags;
    }
    VkSamplerAddressMode ToVulkanAdressMode(SamplerAdressMode samplerAdress) {
        switch (samplerAdress)
        {
        case rhi::REPEAT_ADRESS_MODE: return VK_SAMPLER_ADDRESS_MODE_REPEAT;
        case rhi::MIRRORED_REPEAT_ADRESS_MODE: return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
        case rhi::CLAMP_TO_EDGE_ADRESS_MODE: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        case rhi::CLAMP_TO_BORDER_ADRESS_MODE: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        }
    }VkFilter ToVulkanFilter(SamplerFilter samplerFilter) {
        switch (samplerFilter)
        {
        case rhi::LINEAR_FILTER: return VK_FILTER_LINEAR;
        case rhi::NEAREST_FILTER: return VK_FILTER_NEAREST;
        }
    }
}