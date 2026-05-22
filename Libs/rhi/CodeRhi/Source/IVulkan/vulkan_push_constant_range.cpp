#include "IVulkan/vulkan_push_constant_range.hpp"
#include "IVulkan/enums/vulkan_input_format.hpp"

namespace rhi {
	void VulkanPushConstantRange::Create(size_t size, size_t offset, ShaderStagesFlags stages) {
		pushConstantRange.offset = offset;
		pushConstantRange.size = size;
		pushConstantRange.stageFlags = ToVulkanShaderStage(stages);
	}
}