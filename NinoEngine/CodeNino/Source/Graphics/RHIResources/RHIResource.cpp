#include "Graphics/RHIResources/RHIResource.hpp"

namespace nino_engine {
    IRHIResource::IRHIResource(ERHIResourceType RHIResourceType) {
        resourceType = RHIResourceType;
    }
   
    ERHIResourceType IRHIResource::GetType() const {
        return resourceType;
    }
    const char* IRHIResource::GetDebugName() const {
        return debugName.c_str();
    }
}