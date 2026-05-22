#pragma once
#include "enums/buffer_usage.hpp"
#include "vulkan/vulkan.h"

namespace rhi {
	struct VulkanBufferState {
		VkAccessFlags2 accessFlags = VK_ACCESS_2_NONE;
		VkPipelineStageFlags2 pipelineStage = VK_PIPELINE_STAGE_2_NONE;
	};
	VkBufferUsageFlags ToVulkanBufferUsage(BufferUsage bufferUsage);
	VulkanBufferState ToVulkanBufferState(BufferState bufferState);
	BufferUsage FromVulkanBufferUsage(VkBufferUsageFlags vkBufferUsage);

}