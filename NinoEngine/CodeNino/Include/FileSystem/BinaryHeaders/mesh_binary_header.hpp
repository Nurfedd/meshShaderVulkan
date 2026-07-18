#pragma once
#include <stdint.h>
#include "asset_binary_reader.hpp"
namespace nino_engine {
	struct MeshBinaryHeader : public AssetBinaryHeader {
		uint32_t VerticeCount;
		uint32_t IndicesCount;
		
	};
}