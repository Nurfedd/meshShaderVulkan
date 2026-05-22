#pragma once
#include "Interface/device.hpp"
#include "Interface/device_queue.hpp"
#include "Interface/command_buffer.hpp"

namespace rhi {
	class VulkanCommandPool;
	class CommandPool {
	public :
		virtual ~CommandPool() = default;
		virtual void Create(Device* device, DeviceQueue* _queue) { queue = _queue; };
		virtual void AllocateCommandBuffers(Device* device,CommandBuffer** commandBuffer,uint32_t count) = 0;
		virtual void Destroy(Device* device) = 0;
		virtual VulkanCommandPool& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanCommandPool"); }
	protected :
		DeviceQueue* queue;
	};
}