#pragma once
#include "device.hpp"

namespace rhi {
	class VulkanFence;   
	class Fence {
	public :
		virtual ~Fence() = default;
		virtual void Create(Device* device) = 0;
		virtual void Destroy(Device* device) = 0;
		virtual VulkanFence& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanFence"); }
	};
}

