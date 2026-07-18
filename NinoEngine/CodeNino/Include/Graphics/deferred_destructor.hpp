#pragma once
#include "service.hpp"
#include "service_locator.hpp"
#include "Interface/buffer.hpp"
#include "Interface/fence.hpp"



// this class is used to destroy resources easily and not having trouble with resources that exists over multiple frames
namespace nino_engine {
	class DeferredDestructor : public Service {
	public :
		~DeferredDestructor();
		void SetFrameInFlightCount(uint32_t frameInFlightCount);
		void PushBuffer(rhi::Buffer* buffer,uint32_t frameOffset = 0);
		// should be called when frame begin
		void ClearForCurrentFrame();
		// should be called when frame end
		void AdvanceToNextFrame();
	private :
		void ClearBuffersAtFrame(uint32_t frameIndex);
		//void ClearImagesAtFrame(uint32_t currentFrame);

		// in range [0,frameCount - 1]
		uint32_t currentFrame = 0;
		uint32_t frameInFlightsCount;

		std::vector<std::vector<rhi::Buffer*>> deferredBuffers;
	};
}