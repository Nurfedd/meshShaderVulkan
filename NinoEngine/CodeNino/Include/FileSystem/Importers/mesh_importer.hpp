#pragma once
#include "importer.hpp"

namespace nino_engine {
	enum MeshAttribs {
		MESH_ATTRIB_POSITION = 1 << 0,
		MESH_ATTRIB_NORMAL = 1 << 1,
		MESH_ATTRIB_UV = 1 << 2,
	};
	class MeshImporter : public Importer {
		
	public :

	};
}
