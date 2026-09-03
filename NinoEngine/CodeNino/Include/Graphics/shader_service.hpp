#pragma once
#include "service_locator.hpp"
#include "Graphics/Types/shader_type.hpp"
#include <memory>

namespace nino_engine {
	class ShaderService : public Service {
	public :
		void LoadShaders(rhi::Device* device);
		void OnPreDestroy() override;
	private :
		std::vector<std::unique_ptr<GlobalShader>> shaders;
	};
}