#include "Graphics/graphic_heap.hpp"
#include "Interface/graphic_api.hpp"
#include "Graphics/engine_graphic_resources.hpp"
#include "service_locator.hpp"
#include "engine.hpp"

using namespace rhi;
namespace nino_engine {
	GraphicHeap::GraphicHeap(size_t initialSize) {
		heapBuffer = renderInterface->InitBuffer();

		EngineGraphicResources* graphicsResources = GEngine->EngineServices.Get<EngineGraphicResources>();
		Device* device = graphicsResources->Device;

		heapBuffer->CreateCpu(device, BufferUsage::TRANSFER_DST | BufferUsage::TRANSFER_SRC, nullptr, initialSize);
	}

	GraphicHeap::~GraphicHeap() {
		EngineGraphicResources* graphicsResources = GEngine->EngineServices.Get<EngineGraphicResources>();
		Device* device = graphicsResources->Device;

		renderInterface->DestroyBuffer(heapBuffer, device);
	}

	HeapAllocation GraphicHeap::Allocate(size_t size) {
		EngineGraphicResources* graphicsResources = GEngine->EngineServices.Get<EngineGraphicResources>();
		Device* device = graphicsResources->Device;

		if (heapBuffer->GetBufferSize() + offset < size) {
			heapBuffer->ReallocCpu(device, heapBuffer->GetBufferSize() + offset);
		}
		
		HeapAllocation newHeapAllocation;
		newHeapAllocation.size = size;
		newHeapAllocation.pointer = static_cast<char*>(heapBuffer->GetBufferData()) + offset;

		offset += size;
		return newHeapAllocation;
	}
	void GraphicHeap::Reset() {
		offset = 0;
	}
}