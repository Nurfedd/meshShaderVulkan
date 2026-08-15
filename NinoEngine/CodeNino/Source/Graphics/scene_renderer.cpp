#include "Graphics/scene_renderer.hpp"
#include "Interface/graphic_api.hpp"
#include "service_locator.hpp"
#include "Graphics/engine_graphic_resources.hpp"
using namespace rhi;
namespace nino_engine {
	void SceneRenderer::Create(Device* device) {
		graphicQueue = device->GetGraphicQueue();
		presentQueue = device->GetPresentQueue();
		computeQueue = device->GetComputeQueue();

		EngineGraphicResources* engineGraphicResources = ServiceLocator::Get<EngineGraphicResources>();
		CommandPool* commandPool = engineGraphicResources->GraphicCommandPool;

		uint32_t frameCount = engineGraphicResources->GetFrameInFlightCount();

		graphicCommandBuffers.resize(frameCount);
		for (uint32_t i = 0; i < frameCount; i++) {
			graphicCommandBuffers[i] = renderInterface->InitCommandBuffer();
		}

		commandPool->AllocateCommandBuffers(device, graphicCommandBuffers.data(), frameCount);
	}

	CommandBuffer* SceneRenderer::GetCurrentCommandBuffer() {
		EngineGraphicResources* engineGraphicResources = ServiceLocator::Get<EngineGraphicResources>();
		return graphicCommandBuffers[engineGraphicResources->GetCurrentFrame()];
	}

	void SceneRenderer::Destroy(Device* device) {
		EngineGraphicResources* engineGraphicResources = ServiceLocator::Get<EngineGraphicResources>();
		uint32_t frameCount = engineGraphicResources->GetFrameInFlightCount();

		for (uint32_t i = 0; i < frameCount; i++) {
			renderInterface->DestroyCommandBuffer(graphicCommandBuffers[i]);
		}
		
	}
}