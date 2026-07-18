#pragma once
#include <filesystem>
#include <vector>
#include <string>


namespace nino_engine {
	

/*
* Importer base class used to convert files in nino_engine format for fast load
*/
	class Importer {
	public :
		virtual ~Importer() = default;
		Importer();
		bool Import(std::filesystem::path srcFilePath,std::filesystem::path dstFilePath,bool canOverrideCurrent);
		
		std::vector<std::string> GetSupportedSourceExtensions();
		std::string GetConvertedExtension();
	protected : 
		virtual bool ImportDataFromSource(std::filesystem::path srcFilePath) = 0;
		virtual bool WriteInConvertedFile(std::filesystem::path dstFilePath) = 0;

		virtual std::vector<std::string> DefineSupportedSourceExtensions() = 0;
		virtual std::string DefineConvertedExtensions() = 0;
	private :
		bool SupportSourceExtension(std::string extension);
		bool SupportSourceExtension(std::filesystem::path path);

		std::vector<std::string> sourceFileExtensions;
		std::string convertedExtension;
	};
}