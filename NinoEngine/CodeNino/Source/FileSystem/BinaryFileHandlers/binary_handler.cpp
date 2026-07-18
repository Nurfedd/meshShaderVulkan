#pragma once
#include "FileSystem/BinaryFileHandlers/binary_handler.hpp"

namespace nino_engine {
	bool BinaryFileHandler::OpenRead(const std::filesystem::path& path) {
		if (!stream.is_open()) {
			stream.open(path, std::ios::binary | std::ios::in);
			return stream.is_open();
		}
		return true; // already open
	}
	bool BinaryFileHandler::OpenWrite(const std::filesystem::path& path, bool clearContent) {
		if (!stream.is_open()) {
			std::ios::openmode openMode = std::ios::binary | std::ios::out;
			if (clearContent) {
				openMode |= std::ios::trunc;
			}
			stream.open(path, openMode);
			return stream.is_open();
		}
		return true; // already open
	}

}