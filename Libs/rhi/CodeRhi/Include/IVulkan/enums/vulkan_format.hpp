#pragma once
#include "vulkan/vulkan.h"
#include "enums/format.hpp"
static const VkPresentModeKHR gPresentModeToVk[] = {
	VK_PRESENT_MODE_IMMEDIATE_KHR,
	VK_PRESENT_MODE_MAILBOX_KHR,
	VK_PRESENT_MODE_FIFO_KHR
};

static const VkFormat gFormatToVk[] = {
	VK_FORMAT_UNDEFINED,              // Undefined
	VK_FORMAT_R16G16B16A16_SFLOAT,	 // RGBA16_SFLOAT
	VK_FORMAT_R8_UNORM,				 // R8_UNORM
	VK_FORMAT_R8G8_UNORM,			 // RG8_UNORM
	VK_FORMAT_R8G8B8_UNORM,			 // RGB8_UNORM
	VK_FORMAT_R8G8B8A8_UNORM,        // RGBA8_UNORM
	VK_FORMAT_R16G16B16A16_UNORM,	 // RGBA16_UNORM
	VK_FORMAT_R8_SRGB,				 // R8_SRGB
	VK_FORMAT_R8G8_SRGB,			 // RG8_SRGB
	VK_FORMAT_R8G8B8_SRGB,			 // RBG8_SRGB
	VK_FORMAT_R8G8B8A8_SRGB,         // RGBA8_SRGB
	VK_FORMAT_B8G8R8A8_UNORM,        // BGRA8_UNORM
	VK_FORMAT_B8G8R8A8_SRGB,         // BGRA8_SRGB
	VK_FORMAT_D32_SFLOAT,            // D32_FLOAT
	VK_FORMAT_D24_UNORM_S8_UINT,     // D24_UNORM_S8_UINT
};

static const VkCompareOp gDepthOperatorToVk[] = {
	VK_COMPARE_OP_NEVER,
	VK_COMPARE_OP_LESS,
	VK_COMPARE_OP_EQUAL,
	VK_COMPARE_OP_LESS_OR_EQUAL,
	VK_COMPARE_OP_GREATER,
	VK_COMPARE_OP_NOT_EQUAL,
	VK_COMPARE_OP_GREATER_OR_EQUAL,
	VK_COMPARE_OP_ALWAYS,
};
static const VkColorSpaceKHR gColorSpaceToVk[] = {
	VK_COLORSPACE_SRGB_NONLINEAR_KHR,
};
namespace rhi {
	VkPresentModeKHR ToVulkanPresentMode(PresentMode presentMode);
	PresentMode ConvertVkPresent(VkPresentModeKHR vkPresentMode);
	VkColorSpaceKHR ToVulkanColorSpace(ColorSpace colorSpace);
	ColorSpace FromVulkanColorSpace(VkColorSpaceKHR colorSpace);
	VkFormat ToVulkanFormat(Format format);
	Format FromVulkanFormat(VkFormat format);
	VkCompareOp ToVulkanCompareOp(DepthOperator depthOperator);
}