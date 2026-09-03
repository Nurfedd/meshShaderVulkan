#include "Interface/set_layout.hpp"

namespace rhi {
	void SetLayout::Create(Device* device, SetInfo* setInfos, uint32_t setInfoCount) {
		for (uint32_t i = 0; i < setInfoCount; i++) {
			SetInfo info = setInfos[i];
			layoutInfos[info.descriptorType] += info.count;
		}
	}
}