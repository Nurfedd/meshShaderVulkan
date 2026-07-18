#pragma once
#include <stdint.h>

namespace nino_engine {
	enum AssetType {
		NONE,
		MESH_ASSET,
		TEXTURE_ASSET
	};
	struct AssetBinaryHeader {
		AssetType assetType;
	};
}