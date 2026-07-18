#include "FileSystem/BinaryFileHandlers/asset_binary_handler.hpp"
#include "FileSystem/BinaryHeaders/asset_binary_reader.hpp"
namespace nino_engine {
	AssetType AssetBinaryHandler::ReadFile(const std::filesystem::path& path) {
		if (OpenRead(path)) {
			AssetBinaryHeader assetHeader;
			Read<AssetBinaryHeader>(assetHeader);
			return assetHeader.assetType;
		}
		else {
			return AssetType::NONE;
		}
	}
}