#pragma once
#include "Interface/surface.hpp"
namespace rhi {
	class VulkanPhysicalDevice;
	enum QueueSupportedFlags {
		NONE = 0,
		GRAPHICS_SUPPORT = 1 << 1,
		COMPUTE_SUPPORT = 1 << 2,
	};
	enum QueueType {
		GRAPHIC_QUEUE,
		COMPUTE_QUEUE,
		PRESENT_QUEUE,
	};
	class PhysicalDevice {
	public :
		bool isDiscreteGPU;
		QueueSupportedFlags flags = NONE;
		virtual ~PhysicalDevice() = default;
		virtual bool SupportSurface(Surface* surface) = 0;
		virtual VulkanPhysicalDevice& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanPhysicalDevice"); }

	protected :
		virtual void LoadInfo() = 0;

	};
}