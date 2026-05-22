#include "Interface/graphic_api.hpp"
#include "IVulkan/vulkan_interface.hpp"
#include "Volk/volk.h"
#include <memory>

namespace rhi {
	bool hasInit = false;
	RenderInterface* renderInterface = nullptr;
	RENDER_API renderApi;
	void Init(RENDER_API _renderAPI) {
		volkInitialize();
		if (hasInit)
			return;
		switch (_renderAPI)
		{
		case VULKAN_API:
			hasInit = true;
			renderInterface = new VulkanInterface;
		}
	}

	void Switch(RENDER_API newRenderAPI) {
		if (renderApi == newRenderAPI)
			return;
		if (renderInterface)
			delete renderInterface;
		switch (newRenderAPI)
		{
		case VULKAN_API:
			renderInterface = new VulkanInterface;
			break;
		}
		renderApi = newRenderAPI;
	}

	void Close() {
		if (renderInterface)
			delete renderInterface;
		volkFinalize();
	}

	RENDER_API GetRenderApi() {
		return renderApi;
	}
}