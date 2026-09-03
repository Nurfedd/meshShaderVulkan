#include "Graphics/Global/global_vertex_layout.hpp"
#include "Interface/graphic_api.hpp"
using namespace rhi;
namespace nino_engine {
	GlobalVertexLayout::GlobalVertexLayout(std::vector<VertexInput> vertexInputs, uint32_t _binding, uint32_t _stride) :
		vertexInputs(vertexInputs) , binding(_binding),stride(_stride)
	{}

	void GlobalVertexLayout::Create(Device* device) {
		vertexLayout = renderInterface->InitVertexLayout();
		vertexLayout->Create(vertexInputs.data(), vertexInputs.size(), binding, stride);
	}

	void GlobalVertexLayout::Destroy(Device* device) {
		renderInterface->DestroyVertexLayout(vertexLayout);
	}
}