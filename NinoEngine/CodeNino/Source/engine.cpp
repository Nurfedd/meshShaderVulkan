#include "engine.hpp"
#include "Graphics/engine_graphic_resources.hpp"
#include "Interface/graphic_api.hpp"
#include "task_manager.hpp"
#include <iostream>
using namespace rhi;
using namespace mt;
namespace nino_engine {
	void NinoEngine::Init(GLFWwindow* window) {
		rhi::Init(VULKAN_API);
		EngineGraphicResources* engineResources = ServiceLocator::RegisterService<EngineGraphicResources>(0);
		TaskManager* taskManager = ServiceLocator::RegisterService<TaskManager>(0);

		
		engineResources->Create(window);
		long long int r = 0;

		
		rendererController.Create();
	}

	void NinoEngine::Update() {
		// Build frame data and push to render thread queue
		FrameData frameData{};
		rendererController.PushFrame(frameData);
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