#include "enums/image_type.hpp"

namespace rhi {
	uint32_t GetLayerCountByViewType(ViewType viewType) {
		switch (viewType)
		{
		case rhi::VIEW_2D: return 1;
		case rhi::VIEW_CUBE: return 6; 
		}
	}
}