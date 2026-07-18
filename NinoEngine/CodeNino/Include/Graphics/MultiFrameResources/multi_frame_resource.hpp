#pragma once
#include <stdint.h>
#include <vector>
#include "Graphics/engine_graphic_resources.hpp"
#include "service_locator.hpp"

namespace nino_engine {
	template <typename T>
	class MultiFrameResource {
	public :
		T* GetResourceBasedOnSwapchainFrameIndex() {
			EngineGraphicResources* engineResources = ServiceLocator::Get<EngineGraphicResources>();
			return GetResourceAt(engineResources->GetCurrentFrame());
		};
		T* GetResourceAt(uint32_t index) {return resources[index] };
	private :
		std::vector<T*> resources;
	};
}