#include "Graphics/RHIResources/RHIResource.hpp"

namespace nino_engine {
    IRHIResource::IRHIResource() {
        resourceType = DefineResourceType();
    }
    void IRHIResource::AddRef() {
        refCount.fetch_add(1,std::memory_order_relaxed);
    }
    void IRHIResource::Release() {
        uint32_t prev = refCount.fetch_sub(1, std::memory_order_acq_rel);
        if (prev == 1) {
           
        }
    }
    ERHIResourceType IRHIResource::GetType() const {
        return resourceType;
    }
    const char* IRHIResource::GetDebugName() const {
        return debugName.c_str();
    }
}