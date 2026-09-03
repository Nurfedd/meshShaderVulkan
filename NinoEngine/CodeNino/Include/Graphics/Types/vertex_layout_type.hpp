#pragma once
#include <vector>
#include "Interface/vertex_layout.hpp"
namespace nino_engine {
	class GlobalVertexLayout;

	class VertexLayoutType {
    public:
        using ConstructFunc = GlobalVertexLayout * (*)();

        VertexLayoutType(
            std::vector<rhi::VertexInput> vertexInputs,
            uint32_t stride,
            uint32_t binding, ConstructFunc constructFunc) 
            : stride(stride),binding(binding),vertexInputs(vertexInputs), constructFunc(constructFunc)
        {

            GetTypeList().push_back(this);
        }

        GlobalVertexLayout* ConstructInstance() const { return constructFunc(); }
        const uint32_t GetStride() const { return stride; }
        const uint32_t GetBinding() const { return binding; }
        const std::vector<rhi::VertexInput> GetVertexInputs() { return vertexInputs; }

        static std::vector<VertexLayoutType*>& GetTypeList() {
            static std::vector<VertexLayoutType*> types;
            return types;
        }

    private:
        std::vector<rhi::VertexInput> vertexInputs;
        uint32_t stride;
        uint32_t binding;
        ConstructFunc constructFunc;
	};
}