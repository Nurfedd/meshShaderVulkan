#include "Interface/surface.hpp"
#include "Interface/device.hpp"
namespace rhi {
	bool Surface::SupportPresentMode(PresentMode presentMode) {
		for (PresentMode mode : presentModes) {
			if (mode == presentMode)
				return true;
		}
		return false;
	}

	bool Surface::SupportSurfaceFormat(SurfaceFormat surfaceFormat) {
		for (SurfaceFormat format : surfaceFormats) {
			if (surfaceFormat.format == format.format && surfaceFormat.colorSpace == format.colorSpace)
				return true;
		}
		return false;
	}
}