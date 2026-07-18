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

		
		taskManager->ParallelForWait(80000, [](uint32_t index) mutable {
			if (index == 0) {
				std::cout << "ONLY 0";
			}
		});
		
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