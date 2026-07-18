#pragma once
#include "Graphics/resource.hpp"
#include "nino_core.hpp"
#include <filesystem>
namespace nino_engine {
	/*enum class MeshTypeFlags
	{
		MESHTYPE_VERTEX,
		MESHTYPE_MESHLET,
		MESHTYPE_RAYBLAS
	};
	ENUM_CLASS_FLAGS(MeshTypeFlags);*/


	class Mesh : public IResource {
	public :
		
		void Destroy() override {};
	};
}