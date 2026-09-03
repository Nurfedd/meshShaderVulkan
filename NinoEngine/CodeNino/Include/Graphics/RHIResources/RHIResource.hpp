#pragma once
#include <atomic>
#include <string>
#include "Interface/device.hpp"
#include "Interface/buffer.hpp"
// Bse class

namespace nino_engine {
    enum ERHIResourceType
    {
        RT_Buffer,
        RT_Texture
    };

    class IRHIResource {
    public:
        IRHIResource(ERHIResourceType RHIResourceType);
        virtual ~IRHIResource() = default;
        ERHIResourceType GetType() const;
        const char* GetDebugName() const;
        virtual void Destroy(rhi::Device* device) = 0;
    protected:
        ERHIResourceType resourceType;
        std::string debugName;
    };

    class RHIViewResource : public IRHIResource {

    };
}