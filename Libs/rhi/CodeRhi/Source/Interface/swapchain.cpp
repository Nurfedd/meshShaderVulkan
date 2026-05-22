#include "Interface/swapchain.hpp"

namespace rhi {
	void Swapchain::Create(Device* device, SwapchainCreateInfo swapchainCreateInfo) {
		presentMode = swapchainCreateInfo.presentMode;
		surfaceFormat = swapchainCreateInfo.surfaceFormat;
		surface = swapchainCreateInfo.surface;
	}

	void Swapchain::CreateImages(Device* device) {
		swapchainImageTextures.resize(imageCount);
	}
}