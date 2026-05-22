#pragma once
#include "vulkan/vulkan.h"
#include "enums/synchronization_stage.hpp"

namespace rhi {
    struct VulkanBarrierInfo {
        VkPipelineStageFlags2 stage;
        VkAccessFlags2 access;
    };
    VkPipelineStageFlags ToVulkanPipelineStage(PipelineStageFlags pipelineStage);
    VkPipelineStageFlags2 ToVulkanPipelineStage2(PipelineStageFlags pipelineStage);
    VkAccessFlags2 ToVulkanAccessFlags2(AccessFlags accessFlags);
    PipelineStageFlags FromVulkanPipelineStage(VkPipelineStageFlags vkPipelineStage);
    VkImageLayout ToVulkanImageLayout(ImageLayout imageLayout);
    VulkanBarrierInfo VulkanUsageToBarrier(VkBufferUsageFlags usage);
}