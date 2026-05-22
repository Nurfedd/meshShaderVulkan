#pragma once
#include "enums/rasterizer_state.hpp"
#include "vulkan/vulkan.h"

static constexpr VkPolygonMode gPolygonModeToVk[] = {
		VK_POLYGON_MODE_FILL,   // FILL
		VK_POLYGON_MODE_LINE,   // LINE
		VK_POLYGON_MODE_POINT   // POINT
};

static constexpr VkFrontFace gFrontFaceToVk[] = {
	VK_FRONT_FACE_CLOCKWISE,         // CLOCKWISE
	VK_FRONT_FACE_COUNTER_CLOCKWISE  // COUNTER_CLOCKWISE
};

namespace rhi {
	VkPolygonMode ToVulkanPolygonMode(PolygonMode polygonMode);
	PolygonMode FromVulkanPolygonMode(VkPolygonMode vkPolygonMode);

	VkFrontFace ToVulkanFrontFace(FrontFace frontFace);
	FrontFace FromVulkanFrontFace(VkFrontFace vkFrontFace);

	VkCullModeFlags ToVulkanCullModeFlags(CullModeFlags cullModeFlags);
	CullModeFlags FromVulkanCullModeFlags(VkCullModeFlags vkCullModeFlags);
}