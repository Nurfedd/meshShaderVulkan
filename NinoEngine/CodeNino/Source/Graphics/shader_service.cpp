#include "Graphics/shader_service.hpp"
#include "Graphics/Global/global_shader.hpp"
#include "Graphics/engine_graphic_resources.hpp"
#include "engine.hpp"
using namespace rhi;
namespace nino_engine {
	void ShaderService::LoadShaders(Device* device) {
		std::vector<ShaderType*> shaderTypes = nino_engine::ShaderType::GetTypeList();
		for (ShaderType* shaderType : shaderTypes) {
			std::unique_ptr<GlobalShader> newShader(shaderType->ConstructInstance());
			newShader->Create(device);
			shaders.push_back(std::move(newShader));
		}
	}

	void ShaderService::OnPreDestroy() {
		EngineGraphicResources* graphicsResources = GEngine->EngineServices.Get<EngineGraphicResources>();

		for (std::unique_ptr<GlobalShader>& shader : shaders) {
			shader->Destroy(graphicsResources->Device);
		}
	}
}