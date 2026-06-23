#pragma once
#include <stdint.h>
#include "nino_core.hpp"
namespace rhi {
	enum ViewType {
		VIEW_2D,
		VIEW_CUBE,
	};
    enum ImageUsage {
        NONE_IMAGE_USAGE = 0,
        TRANSFER_SRC_USAGE = 1 << 1,
        TRANSFER_DST_USAGE = 1 << 2,
        SAMPLED_USAGE = 1 << 3,
        STORAGE_USAGE = 1 << 4,
        COLOR_ATTACHMENT_USAGE = 1 << 5,
        DEPTH_STENCIL_ATTACHMENT_USAGE = 1 << 6,
        INPUT_ATTACHMENT_USAGE = 1 << 7,
    };
    ENUM_CLASS_FLAGS(ImageUsage);
    enum SamplerAdressMode {
        REPEAT_ADRESS_MODE,
        MIRRORED_REPEAT_ADRESS_MODE,
        CLAMP_TO_EDGE_ADRESS_MODE,
        CLAMP_TO_BORDER_ADRESS_MODE,
    };
    enum SamplerFilter {
        LINEAR_FILTER,
        NEAREST_FILTER,
    };
    uint32_t GetLayerCountByViewType(ViewType viewType);
}