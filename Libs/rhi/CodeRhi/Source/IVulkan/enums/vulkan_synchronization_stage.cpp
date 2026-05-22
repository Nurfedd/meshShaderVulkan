#include "IVulkan/enums/vulkan_synchronization_stage.hpp"

namespace rhi {
    VkPipelineStageFlags ToVulkanPipelineStage(PipelineStageFlags pipelineStage)
    {
        VkPipelineStageFlags result = 0;

        if (pipelineStage & PIPELINE_STAGE_TOP_OF_PIPE)
            result |= VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

        if (pipelineStage & PIPELINE_STAGE_DRAW_INDIRECT)
            result |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;

        if (pipelineStage & PIPELINE_STAGE_VERTEX_INPUT)
            result |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;

        if (pipelineStage & PIPELINE_STAGE_VERTEX_SHADER)
            result |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;

        if (pipelineStage & PIPELINE_STAGE_FRAGMENT_SHADER)
            result |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

        if (pipelineStage & PIPELINE_STAGE_COMPUTE_SHADER)
            result |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;

        if (pipelineStage & PIPELINE_STAGE_TRANSFER)
            result |= VK_PIPELINE_STAGE_TRANSFER_BIT;

        if (pipelineStage & PIPELINE_STAGE_BOTTOM_OF_PIPE)
            result |= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        if (pipelineStage & PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT)
            result |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        

        return result;
    }
    VkPipelineStageFlags2 ToVulkanPipelineStage2(PipelineStageFlags pipelineStage) {
        VkPipelineStageFlags2 result = 0;

        if (pipelineStage & PIPELINE_STAGE_TOP_OF_PIPE)
            result |= VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT;

        if (pipelineStage & PIPELINE_STAGE_DRAW_INDIRECT)
            result |= VK_PIPELINE_STAGE_2_DRAW_INDIRECT_BIT;

        if (pipelineStage & PIPELINE_STAGE_VERTEX_INPUT)
            result |= VK_PIPELINE_STAGE_2_VERTEX_INPUT_BIT;

        if (pipelineStage & PIPELINE_STAGE_VERTEX_SHADER)
            result |= VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT;

        if (pipelineStage & PIPELINE_STAGE_FRAGMENT_SHADER)
            result |= VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT;

        if (pipelineStage & PIPELINE_STAGE_COMPUTE_SHADER)
            result |= VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;

        if (pipelineStage & PIPELINE_STAGE_TRANSFER)
            result |= VK_PIPELINE_STAGE_2_TRANSFER_BIT;

        if (pipelineStage & PIPELINE_STAGE_BOTTOM_OF_PIPE)
            result |= VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT;

        if (pipelineStage & PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT)
            result |= VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
        
        if (pipelineStage & PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT)
            result |= VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT;
         
        if (pipelineStage & PIPELINE_STAGE_ALL_COMMANDS)
            result |= VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;

        return result;
    }
    VkAccessFlags2 ToVulkanAccessFlags2(AccessFlags flags) {
        VkAccessFlags2 result = 0;

        if (flags & TRANSFER_READ)
            result |= VK_ACCESS_2_TRANSFER_READ_BIT;

        if (flags & TRANSFER_WRITE)
            result |= VK_ACCESS_2_TRANSFER_WRITE_BIT;

        if (flags & SHADER_READ)
            result |= VK_ACCESS_2_SHADER_READ_BIT;

        if (flags & SHADER_WRITE)
            result |= VK_ACCESS_2_SHADER_WRITE_BIT;

        if (flags & VERTEX_READ)
            result |= VK_ACCESS_2_VERTEX_ATTRIBUTE_READ_BIT;

        if (flags & INDEX_READ)
            result |= VK_ACCESS_2_INDEX_READ_BIT;

        if (flags & UNIFORM_READ)
            result |= VK_ACCESS_2_UNIFORM_READ_BIT;

        if (flags & COLOR_ATTACHMENT_WRITE)
            result |= VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
        if (flags & COLOR_ATTACHMENT_READ)
            result |= VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT;

        if (flags & DEPTH_STENCIL_WRITE)
            result |= VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        return result;
    }
    PipelineStageFlags FromVulkanPipelineStage(VkPipelineStageFlags vkPipelineStage)
    {
        int result = PIPELINE_STAGE_NONE;

        if (vkPipelineStage & VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT)
            result = result | PIPELINE_STAGE_TOP_OF_PIPE;

        if (vkPipelineStage & VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT)
            result = result | PIPELINE_STAGE_DRAW_INDIRECT;

        if (vkPipelineStage & VK_PIPELINE_STAGE_VERTEX_INPUT_BIT)
            result = result | PIPELINE_STAGE_VERTEX_INPUT;

        if (vkPipelineStage & VK_PIPELINE_STAGE_VERTEX_SHADER_BIT)
            result = result | PIPELINE_STAGE_VERTEX_SHADER;

        if (vkPipelineStage & VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT)
            result = result | PIPELINE_STAGE_FRAGMENT_SHADER;

        if (vkPipelineStage & VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT)
            result = result | PIPELINE_STAGE_COMPUTE_SHADER;

        if (vkPipelineStage & VK_PIPELINE_STAGE_TRANSFER_BIT)
            result = result | PIPELINE_STAGE_TRANSFER;

        if (vkPipelineStage & VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT)
            result = result | PIPELINE_STAGE_BOTTOM_OF_PIPE;

        return static_cast<PipelineStageFlags>(result);
    }
    VkImageLayout ToVulkanImageLayout(ImageLayout layout) {
        switch (layout)
        {
        case ImageLayout::UNDEFINED:
            return VK_IMAGE_LAYOUT_UNDEFINED;

        case ImageLayout::GENERAL:
            return VK_IMAGE_LAYOUT_GENERAL;

        case ImageLayout::COLOR_ATTACHMENT:
            return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        case ImageLayout::DEPTH_STENCIL_ATTACHMENT:
            return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        case ImageLayout::DEPTH_STENCIL_READ_ONLY:
            return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

        case ImageLayout::SHADER_READ_ONLY:
            return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        case ImageLayout::TRANSFER_SRC:
            return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

        case ImageLayout::TRANSFER_DST:
            return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

        case ImageLayout::PRESENT:
            return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        default:
            return VK_IMAGE_LAYOUT_UNDEFINED;
        }
    }
    VulkanBarrierInfo VulkanUsageToBarrier(VkBufferUsageFlags usage) {
        VulkanBarrierInfo info{};

        if (usage & VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
            info = { VK_PIPELINE_STAGE_2_VERTEX_INPUT_BIT, VK_ACCESS_2_VERTEX_ATTRIBUTE_READ_BIT };
        else if (usage & VK_BUFFER_USAGE_INDEX_BUFFER_BIT)
            info = { VK_PIPELINE_STAGE_2_VERTEX_INPUT_BIT, VK_ACCESS_2_INDEX_READ_BIT };
        else if (usage & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
            info = { VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT, VK_ACCESS_2_UNIFORM_READ_BIT };
        else if (usage & VK_BUFFER_USAGE_STORAGE_BUFFER_BIT)
            info = { VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT, VK_ACCESS_2_SHADER_READ_BIT | VK_ACCESS_2_SHADER_WRITE_BIT };
        else if (usage & VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT)
            info = { VK_PIPELINE_STAGE_2_DRAW_INDIRECT_BIT, VK_ACCESS_2_INDIRECT_COMMAND_READ_BIT };
        else if (usage & VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
            info = { VK_PIPELINE_STAGE_2_TRANSFER_BIT, VK_ACCESS_2_TRANSFER_READ_BIT };
        else if (usage & VK_BUFFER_USAGE_TRANSFER_DST_BIT)
            info = { VK_PIPELINE_STAGE_2_TRANSFER_BIT, VK_ACCESS_2_TRANSFER_WRITE_BIT };

        return info;
    }
}