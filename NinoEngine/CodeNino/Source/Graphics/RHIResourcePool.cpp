#include "Graphics/RHIResourcePool.hpp"
#include "engine.hpp"

namespace nino_engine {
	RHIResourcePool::RHIResourcePool() {
		RendererController* rendererController = GEngine->GetRendererController();
		rendererController->OnPostBeginFrame.Add(this, &RHIResourcePool::OnBeginFrame_RenderThread);
	}

	void RHIResourcePool::OnBeginFrame_RenderThread() {
		for (auto& pair : activeResources) {
			HandleResourcePoolState(pair.first, pair.second);
		}
	}
	void RHIResourcePool::HandleResourcePoolState(IRHIResource* Resource, ResourcePoolStateInfo& ResourcePoolState) {
		EngineGraphicResources* graphicResource = GEngine->EngineServices.Get<EngineGraphicResources>();
		

		RendererController* rendererController = GEngine->GetRendererController();
		uint64_t currentFrame = rendererController->GetFrameCount();

		

		if (ResourcePoolState.flags & ResourcePoolStateFlags::RPS_BeingFreed) {
			if (currentFrame >= ResourcePoolState.freedFrameIndex) {
				ResourcePoolState.flags |= RPS_Freed;
				RemoveFlag<ResourcePoolStateFlags>(ResourcePoolState.flags, RPS_BeingFreed);
			}
		}
		if (ResourcePoolState.flags & (ResourcePoolStateFlags::RPS_Freed | ResourcePoolStateFlags::RPS_DestroyOnFreed)) {
			if (currentFrame == ResourcePoolState.destroyAfterFrameIndex) {
				Resource->Destroy(graphicResource->Device);
				activeResources.erase(Resource);
				delete Resource;
			}
		}
	}

	bool RHIResourcePool::GetResourcePoolStateInfo(IRHIResource* Resource, ResourcePoolStateInfo& ResourcePoolStateInfo) {
		if (Resource == nullptr) {
			return false;
		}
		auto it = activeResources.find(Resource);
		if (it == activeResources.end()) {
			return false;
		}
		ResourcePoolStateInfo = it->second;
		return true;
	}

	void RHIResourcePool::FreeResource(IRHIResource* Resource, uint32_t freedFrameCount) {

		ResourcePoolStateInfo stateInfo;
		if (GetResourcePoolStateInfo(Resource, stateInfo)) {
			RendererController* rendererController = GEngine->GetRendererController();
			stateInfo.flags = RPS_BeingFreed;
			stateInfo.freedFrameIndex = rendererController->GetFrameCount() + freedFrameCount;
			stateInfo.destroyAfterFrameIndex = 0;
		}

	}
	void RHIResourcePool::FreeResourceBasedOnSwapchainImageCount(IRHIResource* Resource) {
		EngineGraphicResources* graphicResource = GEngine->EngineServices.Get<EngineGraphicResources>();
		uint32_t frameInFlightCount = graphicResource->GetFrameInFlightCount();
		FreeResource(Resource, frameInFlightCount);
	}

	void RHIResourcePool::FreeResourceAndDestroy(IRHIResource* Resource, uint32_t freedFrameCount) {
		FreeResourceAndDestroyAfterFrameCount(Resource, freedFrameCount,0);
	}
	
	void RHIResourcePool::FreeResourceAndDestroyAfterFrameCount(IRHIResource* Resource,uint32_t freedFrameCount, uint32_t destroyFrameCount) {
		ResourcePoolStateInfo stateInfo;
		if (GetResourcePoolStateInfo(Resource, stateInfo)) {
			RendererController* rendererController = GEngine->GetRendererController();
			stateInfo.flags = RPS_BeingFreed | RPS_DestroyOnFreed;
			stateInfo.freedFrameIndex = rendererController->GetFrameCount();
			stateInfo.destroyAfterFrameIndex = rendererController->GetFrameCount() + destroyFrameCount;
		}
	}
}