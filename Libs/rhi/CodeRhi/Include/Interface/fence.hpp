#pragma once
#include "device.hpp"

namespace rhi {
	class VulkanFence;   
	class Fence {
	public :
		virtual ~Fence() = default;
		virtual void Create(Device* device,bool initialReady = true) = 0;
		virtual void Destroy(Device* device) = 0;
		virtual bool IsReady(Device* device) = 0;
		virtual VulkanFence& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanFence"); }
	};
}

