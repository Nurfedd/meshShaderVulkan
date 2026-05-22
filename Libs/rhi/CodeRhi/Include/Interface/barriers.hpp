#pragma once
#include "enums/synchronization_stage.hpp"
#include "texture.hpp"

namespace rhi {
	class Buffer;
	class MemoryBarrier {
	public :
		
		PipelineStageFlags srcStageMask;
		AccessFlags srcAccessMask;
		PipelineStageFlags dstStageMask;
		AccessFlags dstAccessMask;

		void SetStages(PipelineStageFlags srcStage, AccessFlags srcAccess, PipelineStageFlags dstStage, AccessFlags dstAccess);
	};


	class BufferBarrier : public MemoryBarrier {
	public :
		Buffer* buffer;
	};
	
	class ImageBarrier : public MemoryBarrier {
	public:
		Texture* image;
		ImageLayout oldLayout;
		ImageLayout newLayout;
	};
}