#include "Interface/barriers.hpp"

namespace rhi {
	void MemoryBarrier::SetStages(PipelineStageFlags srcStage, AccessFlags srcAccess, PipelineStageFlags dstStage, AccessFlags dstAccess) {
		srcStageMask = srcStage;
		srcAccessMask = srcAccess;
		dstStageMask = dstStage;
		dstAccessMask = dstAccess;
	}
}