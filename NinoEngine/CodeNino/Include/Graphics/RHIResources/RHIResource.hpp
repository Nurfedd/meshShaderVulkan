#pragma once
#include <atomic>
#include <string>
#include "Interface/device.hpp"
// Bse class

namespace nino_engine {
    enum ERHIResourceType
    {
        Buffer,
        Texture
    };

    class IRHIResource {
    public:
        IRHIResource();
        virtual ~IRHIResource() = default;
        void AddRef();
        void Release();
        ERHIResourceType GetType() const;
        const char* GetDebugName() const;
        virtual void Destroy(rhi::Device* device) = 0;
    protected:
        std::atomic<uint32_t> refCount{ 0 };
        ERHIResourceType resourceType;
        std::string debugName;

        virtual ERHIResourceType DefineResourceType() = 0;
    };
}