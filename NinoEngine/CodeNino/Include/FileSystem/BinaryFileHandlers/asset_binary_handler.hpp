#pragma once
#include "binary_handler.hpp"
#include "FileSystem/BinaryHeaders/asset_binary_reader.hpp"

namespace nino_engine {
	class AssetBinaryHandler : public BinaryFileHandler {
	public :
		AssetType ReadFile(const std::filesystem::path& path);
		//bool WriteFile(const std::filesystem::path& path,bina)
	};
}