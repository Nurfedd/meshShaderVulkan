#pragma once
#include "Graphics/Types/vertex_layout_type.hpp"
#include "Interface/device.hpp"
#include "Interface/vertex_layout.hpp"


namespace nino_engine {
    class GlobalVertexLayout {
    public:
        virtual ~GlobalVertexLayout() = default;

        GlobalVertexLayout(std::vector<rhi::VertexInput> vertexInputs, uint32_t _binding, uint32_t _stride);

        void Create(rhi::Device* Device);
        void Destroy(rhi::Device* Device);

    private:
        std::vector<rhi::VertexInput> vertexInputs;
        uint32_t binding;
        uint32_t stride;
        rhi::VertexLayout* vertexLayout = nullptr;
    };
}

#define BEGIN_VERTEX_LAYOUT(VertexLayoutClass) \
public: \
    VertexLayoutClass() : nino_engine::GlobalVertexLayout(StaticType.GetVertexInputs(), StaticType.GetBinding(),StaticType.GetStride()) {} \
    static nino_engine::GlobalVertexLayout* ConstructInstance() { return new VertexLayoutClass(); } \
    static nino_engine::VertexLayoutType StaticType;

#define VERTEX_INPUT(Location, Offset, VertexInputFormat) \
    rhi::VertexInput(Location,Offset,VertexInputFormat)

#define IMPLEMENT_VERTEX_LAYOUT(VertexLayoutClass,Stride,Binding,...)\
    nino_engine::VertexLayoutType VertexLayoutClass::StaticType( \
        std::vector<VertexInput>{__VA_ARGS__}, \
        Stride, \
        Binding, \
        &VertexLayoutClass::ConstructInstance\
    );\
