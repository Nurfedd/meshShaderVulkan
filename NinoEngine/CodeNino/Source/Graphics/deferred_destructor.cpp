#include "Graphics/deferred_destructor.hpp"
#include "Interface/graphic_api.hpp"
#include "Graphics/engine_graphic_resources.hpp"
#include "engine.hpp"
using namespace rhi;

namespace nino_engine {
	void DeferredDestructor::SetFrameInFlightCount(uint32_t _frameInFlightCount) {
		frameInFlightsCount = _frameInFlightCount;
	}

	void DeferredDestructor::PushBuffer(Buffer* buffer,uint32_t frameOffset) {
		frameOffset = std::min(frameOffset, frameInFlightsCount - 1);
		deferredBuffers[currentFrame + (frameOffset % frameInFlightsCount)].push_back(buffer);
	}

	void DeferredDestructor::ClearForCurrentFrame() {
		ClearBuffersAtFrame(currentFrame);
	}
	void DeferredDestructor::AdvanceToNextFrame() {
		currentFrame = (currentFrame + 1) % frameInFlightsCount;
	}

	void DeferredDestructor::ClearBuffersAtFrame(uint32_t frameIndex) {
		EngineGraphicResources* graphicsResources = GEngine->EngineServices.Get<EngineGraphicResources>();
		
		for (Buffer* buffer : deferredBuffers[frameIndex]) {
			renderInterface->DestroyBuffer(buffer, graphicsResources->Device);
		}
		deferredBuffers[frameIndex].clear();
	}

	DeferredDestructor::~DeferredDestructor() {
		for (uint32_t frameIndex = 0; frameIndex < frameInFlightsCount; frameIndex++) {
			ClearBuffersAtFrame(frameIndex);
		}
	}
}