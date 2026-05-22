#pragma once
#include <exception>
#include "command_buffer.hpp"
#include "enums/synchronization_stage.hpp"

namespace rhi {
	class VulkanDeviceQueue;
	class Fence;
	class Semaphore;
	class Swapchain;
	struct SemaphoreSubmitInfo {
		Semaphore** semaphores;
		uint32_t semaphoreCount = 0;
		PipelineStageFlags* stageFlags;
		uint64_t* timelineValues;
	};

	struct SubmitInfo {
		SemaphoreSubmitInfo waitSemaphoreInfo;
		SemaphoreSubmitInfo signalSemaphoreInfo;
	};

	struct PresentInfo {
		Semaphore** waitSemaphores;
		uint32_t waitSemaphoreCount;
		Swapchain* swapchain;
	};


	class DeviceQueue {
	public :
		virtual ~DeviceQueue() = default;
		virtual void Submit(CommandBuffer** commandsBuffer,uint32_t commandBufferCount,SubmitInfo submitInfo,Fence* fence) = 0;
		virtual void Submit(CommandBuffer** commandsBuffer, uint32_t commandBufferCount, Fence* fence) = 0;
		virtual bool Present(PresentInfo presentInfo) = 0;
		virtual VulkanDeviceQueue& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanDeviceQueue"); };
	private :

	};
}