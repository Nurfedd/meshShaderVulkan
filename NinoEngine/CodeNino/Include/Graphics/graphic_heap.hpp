#pragma once
#include "Interface/buffer.hpp"

// this class create a staging buffer to be used to copy into others buffer like local memory
// shouldn't be created every frame but use Reset() to reset the heap.
namespace nino_engine {
	struct HeapAllocation {
		void* pointer;
		uint32_t size;
	};
	class GraphicHeap {
	public :
		GraphicHeap(size_t initialSize);
		~GraphicHeap();

		HeapAllocation Allocate(size_t size);
		void Reset();
	private :
		rhi::Buffer* heapBuffer = nullptr;
		size_t offset = 0;
	};
}