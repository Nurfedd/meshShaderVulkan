#pragma once
#include "enums/input_fomat.hpp"
#include <stdint.h>
#include <exception>

namespace rhi {
	class VulkanPushConstantRange;
	class PushConstantRange {
	public :
		virtual ~PushConstantRange() = default;
		virtual void Create(size_t size,size_t offset,ShaderStagesFlags stages) = 0;
		virtual VulkanPushConstantRange& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanSurface"); }
	protected :

	};
}