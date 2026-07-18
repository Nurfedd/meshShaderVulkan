#include "FileSystem/Importers/importer.hpp"
#include <ranges>
namespace nino_engine {
	Importer::Importer() {
		convertedExtension = DefineConvertedExtensions();
		sourceFileExtensions = DefineSupportedSourceExtensions();
	}
	bool Importer::Import(std::filesystem::path srcFilePath, std::filesystem::path dstFilePath,bool canOverrideCurrent) {
		if (!std::filesystem::exists(srcFilePath))
			return false;
		if (!canOverrideCurrent && std::filesystem::exists(dstFilePath))
			return true;
		if (SupportSourceExtension(srcFilePath)) {
			if (ImportDataFromSource(srcFilePath)) {
				return WriteInConvertedFile(dstFilePath);
			}
		}
		return false;
	}
	std::vector<std::string> Importer::GetSupportedSourceExtensions() {
		return sourceFileExtensions;
	}
	std::string Importer::GetConvertedExtension() {
		return convertedExtension;
	}

	bool Importer::SupportSourceExtension(std::string extension) {
		return std::ranges::find(sourceFileExtensions, extension) != sourceFileExtensions.end();
	}
	bool Importer::SupportSourceExtension(std::filesystem::path path) {
		return SupportSourceExtension(path.extension().string());
	}
}