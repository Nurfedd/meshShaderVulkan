#include "Interface/device.hpp"

namespace rhi {
	void Device::Create(Instance* instance,PhysicalDevice* _physicalDevice, Surface* surface) {
		physicalDevice = _physicalDevice;
	}
	void Device::Destroy() {
		for (auto pair : queues) {
			delete pair.second;
		}
	}
}