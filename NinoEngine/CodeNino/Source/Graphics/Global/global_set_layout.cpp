#include "Graphics/Global/global_set_layout.hpp"
#include "Interface/graphic_api.hpp"
using namespace rhi;
namespace nino_engine {
	GlobalSetLayout::GlobalSetLayout(std::vector<rhi::SetInfo> setInfos) : setInfos(setInfos) {}

	void GlobalSetLayout::Create(Device* Device) {
		setLayout = renderInterface->InitSetLayout();
		setLayout->Create(Device, setInfos.data(), setInfos.size());
	}

	void GlobalSetLayout::Destroy(rhi::Device* Device) {
		renderInterface->DestroySetLayout(setLayout, Device);
	}
}