#include "IVulkan/enums/vulkan_rasterizer_state.hpp"

namespace rhi {

	VkPolygonMode ToVulkanPolygonMode(PolygonMode polygonMode) {
		return gPolygonModeToVk[static_cast<uint32_t>(polygonMode)];
	}

	PolygonMode FromVulkanPolygonMode(VkPolygonMode vkPolygonMode) {
		switch (vkPolygonMode) {
		case VK_POLYGON_MODE_FILL:  return FILL;
		case VK_POLYGON_MODE_LINE:  return LINE;
		case VK_POLYGON_MODE_POINT: return POINT;
		default: return FILL;
		}
	}

	VkFrontFace ToVulkanFrontFace(FrontFace frontFace) {
		return gFrontFaceToVk[static_cast<uint32_t>(frontFace)];
	}

	FrontFace FromVulkanFrontFace(VkFrontFace vkFrontFace) {
		switch (vkFrontFace) {
		case VK_FRONT_FACE_CLOCKWISE: return CLOCKWISE;
		case VK_FRONT_FACE_COUNTER_CLOCKWISE: return COUNTER_CLOCKWISE;
		default: return CLOCKWISE;
		}
	}

	VkCullModeFlags ToVulkanCullModeFlags(CullModeFlags cullModeFlags) {
		VkCullModeFlags flags = 0;

		if (cullModeFlags & BACK_FACE)
			flags |= VK_CULL_MODE_BACK_BIT;

		if (cullModeFlags & FRONT_FACE)
			flags |= VK_CULL_MODE_FRONT_BIT;

		return flags;
	}

	CullModeFlags FromVulkanCullModeFlags(VkCullModeFlags vkCullModeFlags) {
		int flags = CULL_NONE;

		if (vkCullModeFlags & VK_CULL_MODE_BACK_BIT)
			flags = flags | BACK_FACE;

		if (vkCullModeFlags & VK_CULL_MODE_FRONT_BIT)
			flags = flags | FRONT_FACE;

		return static_cast<CullModeFlags>(flags);
	}

}