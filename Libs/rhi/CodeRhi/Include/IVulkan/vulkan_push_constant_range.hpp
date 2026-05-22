#pragma once
#include "Interface/push_constant_range.hpp"
#include "vulkan/vulkan.h"
namespace rhi {
	class VulkanPushConstantRange : public PushConstantRange {
	public :
		void Create(size_t size, size_t offset, ShaderStagesFlags stages) override;
		VulkanPushConstantRange& API_VULKAN() override { return *(this); }
		VkPushConstantRange GetRange() const { return pushConstantRange; }
	private :
		VkPushConstantRange pushConstantRange;
	};
}