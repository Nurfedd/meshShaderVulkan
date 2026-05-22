#pragma once
#include "Interface/semaphore.hpp"
#include "vulkan/vulkan.h"

namespace rhi {
	class VulkanSemaphore : public Semaphore {
	public :
		void CreateBinary(Device* device) override;
		void CreateTimeline(Device* device, uint64_t initialValue) override;
		uint64_t GetTimelineValue(Device* device) override;
		void Destroy(Device* device) override;
		VulkanSemaphore& API_VULKAN() override { return *(this); }
		VkSemaphore GetSemaphore() { return semaphore; }
		bool IsBinary() override { return isBinary; }
	protected :
		VkSemaphore semaphore;
		bool isBinary;
	};
}