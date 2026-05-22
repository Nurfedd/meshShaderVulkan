#include "Interface/instance.hpp"

namespace rhi {
	std::vector<PhysicalDevice*> Instance::GetPhysicalDevices() {
		if (!hasLoadedDevice)
			LoadPhysicalDevices();
		return physicalDeviceViews;
	}
}