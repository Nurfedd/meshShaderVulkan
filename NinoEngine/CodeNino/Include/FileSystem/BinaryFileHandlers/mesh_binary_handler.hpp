#pragma once
#include "binary_handler.hpp"
#include "FileSystem/BinaryHeaders/mesh_binary_header.hpp"
#include "FileSystem/Importers/mesh_importer.hpp"

namespace nino_engine {
	
	class MeshBinaryHandler : public BinaryFileHandler {
	public :
		 
	protected :
		MeshBinaryHeader meshBinaryHeader;
		MeshAttribs importMeshAttribs;
		
	};
}