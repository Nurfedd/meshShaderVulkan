#pragma once
#include <exception>

#include <vector>
namespace rhi {
	class VulkanInstance;
	class PhysicalDevice;
	class Instance {
	public :
		virtual ~Instance() = default;
		virtual void Create(const char* applicationName) = 0;
		virtual void Destroy() = 0;
		std::vector<PhysicalDevice*> GetPhysicalDevices();
		virtual VulkanInstance& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanInstance"); }

	protected :
		bool hasLoadedDevice = false;
		std::vector<PhysicalDevice*> physicalDeviceViews;
		virtual void LoadPhysicalDevices() = 0;
		
	};
}