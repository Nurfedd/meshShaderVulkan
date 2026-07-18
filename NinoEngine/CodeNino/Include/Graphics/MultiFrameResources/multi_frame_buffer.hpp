#pragma once
#include "multi_frame_resource.hpp"
#include "Interface/buffer.hpp"

namespace nino_engine {
	template<>
	class MultiFrameResource<rhi::Buffer*> {
		void CreateCpu(rhi::Device* device, rhi::BufferUsage usage, size_t size);
		void CreateGpu();
	};
}