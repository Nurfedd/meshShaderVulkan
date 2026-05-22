#pragma once
#include "enums/input_fomat.hpp"
#include "vulkan/vulkan.h"

static const VkFormat gInputFormatToVulkan[] = {
    VK_FORMAT_R32_SFLOAT,
    VK_FORMAT_R32G32_SFLOAT,
    VK_FORMAT_R32G32B32_SFLOAT,
    VK_FORMAT_R32G32B32A32_SFLOAT,

    VK_FORMAT_R32_SINT,
    VK_FORMAT_R32G32_SINT,
    VK_FORMAT_R32G32B32_SINT,
    VK_FORMAT_R32G32B32A32_SINT,

    VK_FORMAT_R32_UINT,
    VK_FORMAT_R32G32_UINT,
    VK_FORMAT_R32G32B32_UINT,
    VK_FORMAT_R32G32B32A32_UINT,

    VK_FORMAT_R64_UINT,
    VK_FORMAT_R64G64_UINT,
    VK_FORMAT_R64G64B64A64_UINT,
};
static const VkVertexInputRate gInputRateToVulkan[] = {
    VK_VERTEX_INPUT_RATE_VERTEX,
    VK_VERTEX_INPUT_RATE_INSTANCE,
};

static const VkDescriptorType gDescriptorTypeToVulkan[] = {
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
};



namespace rhi {
	VkFormat ToVulkanInputFormat(InputFormat inputFormat);
	InputFormat FromVulkanInputFormat(VkFormat vkFormat);

    VkVertexInputRate ToVulkanInputRate(InputRate inputRate);
    InputRate FromVulkanInputRate(VkVertexInputRate vkInputrate);

    VkDescriptorType ToVulkanDecriptorType(DescriptorType descriptorType);
    DescriptorType FromVulkanDescriptorType(VkDescriptorType vkDescriptorType);

    VkShaderStageFlags ToVulkanShaderStage(ShaderStagesFlags shaderStages);
    ShaderStagesFlags FromVulkanShaderStages(VkShaderStageFlags vkShaderStages);

    VkIndexType ToVulkanIndexType(IndexType indexType);
}