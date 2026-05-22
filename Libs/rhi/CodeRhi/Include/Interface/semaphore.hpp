#pragma once
#include "device.hpp"

namespace rhi {
	class VulkanSemaphore;
	enum SemaphoreType {
		BINARY_SEMAPHORE,
		TIMELINE_SEMAPHORE,
	};
	
	class Semaphore {
	public:
		virtual ~Semaphore() = default;
		virtual void CreateBinary(Device* device) = 0;
		virtual void CreateTimeline(Device* device, uint64_t initialValue) = 0;
		virtual void Destroy(Device* device) = 0;
		virtual uint64_t GetTimelineValue(Device* device) = 0;
		virtual VulkanSemaphore& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanSemaphore"); }
		virtual bool IsBinary() = 0;
	
	};
}
