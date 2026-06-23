#include "engine.hpp"
#include "Graphics/engine_graphic_resources.hpp"
#include "Interface/graphic_api.hpp"
using namespace rhi;
namespace nino_engine {
	void NinoEngine::Init(GLFWwindow* window) {
		rhi::Init(VULKAN_API);
		EngineGraphicResources* engineResources = ServiceLocator::RegisterService<EngineGraphicResources>();
		engineResources->Create(window);
		rendererController.Create();
		
	}

	

	bool NinoEngine::BeginFrame() {
		return rendererController.BeginFrame();
	}

	void NinoEngine::RenderFrame() {
		rendererController.RenderFrame();
	}

	

	void NinoEngine::Destroy() {
		EngineGraphicResources* engineResources = ServiceLocator::Get<EngineGraphicResources>();
		Device* device = engineResources->Device;
		uint32_t frameCount = engineResources->GetFrameInFlightCount();

		device->WaitIdle();
		rendererController.Destroy();
		ServiceLocator::Clear();
	}
}