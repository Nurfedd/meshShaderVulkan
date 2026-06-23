#include "Graphics/resource_manager.hpp"

namespace nino_engine {
	ResourceManager::~ResourceManager() {
		for (std::unique_ptr<IResource>& resource : resources) {
			resource->Destroy();
		}
	}
}