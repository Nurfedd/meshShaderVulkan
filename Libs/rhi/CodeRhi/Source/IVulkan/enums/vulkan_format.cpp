#include "IVulkan/enums/vulkan_format.hpp"

namespace rhi {
	VkPresentModeKHR ToVulkanPresentMode(PresentMode presentMode) {
		return gPresentModeToVk[static_cast<uint32_t>(presentMode)];
	}
	PresentMode ConvertVkPresent(VkPresentModeKHR vkPresentMode) {
		switch (vkPresentMode)
		{
		case VK_PRESENT_MODE_IMMEDIATE_KHR: return IMMEDIATE_MODE;
		case VK_PRESENT_MODE_MAILBOX_KHR: return MAILBOX_MODE;
		case VK_PRESENT_MODE_FIFO_KHR: return FIFO_MODE;
		default: return FIFO_MODE;
		}
	}
	VkColorSpaceKHR ToVulkanColorSpace(ColorSpace colorSpace) {
		return gColorSpaceToVk[static_cast<uint32_t>(colorSpace)];
	}
	ColorSpace FromVulkanColorSpace(VkColorSpaceKHR colorSpace) {
		switch (colorSpace)
		{
		case VK_COLOR_SPACE_SRGB_NONLINEAR_KHR: return SRGB_NON_LINEAR;
		default: return SRGB_NON_LINEAR;
		}
	}
	VkFormat ToVulkanFormat(Format format)
	{
		return gFormatToVk[static_cast<uint32_t>(format)];
	}
	Format FromVulkanFormat(VkFormat format)
	{
		switch (format)
		{
		case VK_FORMAT_R16G16B16A16_SFLOAT:return Format::RGBA16_SFLOAT;
		case VK_FORMAT_R8_UNORM: return Format::R8_UNORM;
		case VK_FORMAT_R8G8_UNORM: return Format::RG8_UNORM;
		case VK_FORMAT_R8G8B8_UNORM: return Format::RGB8_UNORM;
		case VK_FORMAT_R8G8B8A8_UNORM: return Format::RGBA8_UNORM;
		case VK_FORMAT_R16G16B16A16_UNORM: return Format::RGBA16_UNORM;
		case VK_FORMAT_R8_SRGB:  return Format::R8_SRGB;
		case VK_FORMAT_R8G8_SRGB:  return Format::RG8_SRGB;
		case VK_FORMAT_R8G8B8_SRGB:  return Format::RGB8_SRGB;
		case VK_FORMAT_R8G8B8A8_SRGB:  return Format::RGBA8_SRGB;
		case VK_FORMAT_B8G8R8A8_UNORM: return Format::BGRA8_UNORM;
		case VK_FORMAT_B8G8R8A8_SRGB:  return Format::BGRA8_SRGB;
		case VK_FORMAT_D32_SFLOAT:     return Format::D32_FLOAT;
		case VK_FORMAT_D24_UNORM_S8_UINT: return Format::D24_UNORM_S8_UINT;
		default: return Format::Undefined;
		}
	}
	VkCompareOp ToVulkanCompareOp(DepthOperator depthOperator) {
		return gDepthOperatorToVk[static_cast<uint32_t>(depthOperator)];
	}
}