#include "IVulkan/enums/vulkan_input_format.hpp"

namespace rhi {
	VkFormat ToVulkanInputFormat(InputFormat inputFormat) {
        return gInputFormatToVulkan[static_cast<uint32_t>(inputFormat)];
	}
	InputFormat FromVulkanInputFormat(VkFormat format) {
        switch (format)
        {
        case VK_FORMAT_R32_SFLOAT: return FLOAT1;
        case VK_FORMAT_R32G32_SFLOAT: return FLOAT2;
        case VK_FORMAT_R32G32B32_SFLOAT: return FLOAT3;
        case VK_FORMAT_R32G32B32A32_SFLOAT: return FLOAT4;

        case VK_FORMAT_R32_SINT: return INT1;
        case VK_FORMAT_R32G32_SINT: return INT2;
        case VK_FORMAT_R32G32B32_SINT: return INT3;
        case VK_FORMAT_R32G32B32A32_SINT: return INT4;

        case VK_FORMAT_R32_UINT: return UINT32_1;
        case VK_FORMAT_R32G32_UINT: return UINT32_2;
        case VK_FORMAT_R32G32B32_UINT: return UINT32_3;
        case VK_FORMAT_R32G32B32A32_UINT: return UINT32_4;

        case VK_FORMAT_R64_UINT: return UINT64_1;
        case VK_FORMAT_R64G64_UINT: return UINT64_2;
        case VK_FORMAT_R64G64B64A64_UINT: return UINT64_4;

        }
	}
    VkVertexInputRate ToVulkanInputRate(InputRate inputRate) {
        return gInputRateToVulkan[static_cast<uint32_t>(inputRate)];
    }
    InputRate FromVulkanInputRate(VkVertexInputRate inputRate) {
        switch (inputRate)
        {
        case VK_VERTEX_INPUT_RATE_VERTEX: return VERTEX_RATE;
        case VK_VERTEX_INPUT_RATE_INSTANCE: return INSTANCE_RATE;
        }
    }

    VkDescriptorType ToVulkanDecriptorType(DescriptorType descriptorType) {
        return gDescriptorTypeToVulkan[static_cast<uint32_t>(descriptorType)];
    }
    DescriptorType FromVulkanDescriptorType(VkDescriptorType vkDescriptorType) {
        switch (vkDescriptorType)
        {
        case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: return DESCRIPTOR_SAMPLED_TEXTURE;
        case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER: return DESCRIPTOR_UNIFORM_BUFFER;
        case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER: return DESCRIPTOR_STORAGE_BUFFER;
        }
    }

    VkShaderStageFlags ToVulkanShaderStage(ShaderStagesFlags shaderStages)
    {
        VkShaderStageFlags vkFlags = 0;

        if (shaderStages & rhi::VERTEX_STAGE)
            vkFlags |= VK_SHADER_STAGE_VERTEX_BIT;

        if (shaderStages & rhi::FRAGMENT_STAGE)
            vkFlags |= VK_SHADER_STAGE_FRAGMENT_BIT;

        if (shaderStages & rhi::COMPUTE_STAGE)
            vkFlags |= VK_SHADER_STAGE_COMPUTE_BIT;


        return vkFlags;
    }
    ShaderStagesFlags FromVulkanShaderStages(VkShaderStageFlags vkFlags)
    {
        int stages = 0;

        if (vkFlags & VK_SHADER_STAGE_VERTEX_BIT)
            stages |= rhi::VERTEX_STAGE;

        if (vkFlags & VK_SHADER_STAGE_FRAGMENT_BIT)
            stages |= rhi::FRAGMENT_STAGE;

        if (vkFlags & VK_SHADER_STAGE_COMPUTE_BIT)
            stages |= rhi::COMPUTE_STAGE;


        return static_cast<ShaderStagesFlags>(stages);
    }
    VkIndexType ToVulkanIndexType(IndexType indexType) {
        switch (indexType)
        {
        case INDEX_TYPE_UINT_8: return VK_INDEX_TYPE_UINT8;
        case INDEX_TYPE_UINT_16:return VK_INDEX_TYPE_UINT16;
        case INDEX_TYPE_UINT_32:return VK_INDEX_TYPE_UINT32;
        }
    }
}