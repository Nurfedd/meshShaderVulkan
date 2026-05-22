#pragma once
#include "Interface/physical_device.hpp"
#include "Interface/surface.hpp"
#include "Interface/device_queue.hpp"
#include "Interface/instance.hpp"
#include <unordered_map>

namespace rhi {
	class VulkanDevice;
	class Fence;
	class Device {
	public:
		virtual ~Device() = default;
		virtual void Create(Instance* instance,PhysicalDevice* _physicalDevice, Surface* surface);
		virtual void Destroy();
		virtual void WaitIdle() = 0;
		virtual VulkanDevice& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanDevice"); }
		virtual void WaitForFences(Fence** fences, uint32_t count,bool waitForAll ,uint64_t timeout = UINT64_MAX) = 0;
		virtual void ResetFences(Fence** fences, uint32_t count) = 0;
		DeviceQueue* GetQueue(QueueType queueType) { return queues[queueType]; }
		PhysicalDevice* GetPhysicalDevice() { return physicalDevice; }
	protected:
		std::unordered_map<QueueType, DeviceQueue*> queues;
		PhysicalDevice* physicalDevice;
	};
}