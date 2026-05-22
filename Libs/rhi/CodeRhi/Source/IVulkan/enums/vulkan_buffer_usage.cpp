#include "IVulkan/enums/vulkan_buffer_usage.hpp"
#include "vulkan/vulkan.h"

namespace rhi {
	VkBufferUsageFlags ToVulkanBufferUsage(BufferUsage bufferUsage) {
		VkBufferUsageFlags flags = 0;
		if (bufferUsage & UNIFORM_BUFFER)
			flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		if (bufferUsage & STORAGE_BUFFER)
			flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		if (bufferUsage & INDEX_BUFFER)
			flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		if (bufferUsage & VERTEX_BUFFER)
			flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        if (bufferUsage & INDIRECT_COMMAND_BUFFR)
            flags |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;

		return flags;
	}

	BufferUsage FromVulkanBufferUsage(VkBufferUsageFlags vkBufferUsage) {
		int flags = 0;

		if (vkBufferUsage & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
			flags |= UNIFORM_BUFFER;
		if (vkBufferUsage & VK_BUFFER_USAGE_STORAGE_BUFFER_BIT)
			flags |= STORAGE_BUFFER;
		if (vkBufferUsage & VK_BUFFER_USAGE_INDEX_BUFFER_BIT)
			flags |= INDEX_BUFFER;
		if (vkBufferUsage & VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
			flags |= VERTEX_BUFFER;
        if (vkBufferUsage & VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT)
            flags |= INDIRECT_COMMAND_BUFFR;

		return static_cast<BufferUsage>(flags);
	}
    VulkanBufferState ToVulkanBufferState(BufferState bufferState)
    {
        switch (bufferState)
        {
        case BUFFER_STATE_COMPUTE_WRITE:
            return {
                VK_ACCESS_2_SHADER_WRITE_BIT,
                VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT
            };

        case BUFFER_STATE_COMPUTE_READ:
            return {
                VK_ACCESS_2_SHADER_SAMPLED_READ_BIT | VK_ACCESS_2_SHADER_STORAGE_READ_BIT,
                VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT
            };

        case BUFFER_STATE_VERTEX_BUFFER:
            return {
                VK_ACCESS_2_VERTEX_ATTRIBUTE_READ_BIT,
                VK_PIPELINE_STAGE_2_VERTEX_INPUT_BIT
            };

        case BUFFER_STATE_INDEX_BUFFER:
            return {
                VK_ACCESS_2_INDEX_READ_BIT,
                VK_PIPELINE_STAGE_2_INDEX_INPUT_BIT
            };

        case BUFFER_STATE_INDIRECT_DRAW:
            return {
                VK_ACCESS_2_INDIRECT_COMMAND_READ_BIT,
                VK_PIPELINE_STAGE_2_DRAW_INDIRECT_BIT
            };

        case BUFFER_STATE_TRANSFER_SRC:
            return {
                VK_ACCESS_2_TRANSFER_READ_BIT,
                VK_PIPELINE_STAGE_2_TRANSFER_BIT
            };

        case BUFFER_STATE_TRANSFER_DST:
            return {
                VK_ACCESS_2_TRANSFER_WRITE_BIT,
                VK_PIPELINE_STAGE_2_TRANSFER_BIT
            };

        case BUFFER_STATE_UNDEFINED:
            return {
                0,
                VK_PIPELINE_STAGE_2_NONE
            };
        }
    }
}