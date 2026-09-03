#include "Graphics/Global/global_shader.hpp"
#include "Interface/graphic_api.hpp"

using namespace rhi;
namespace nino_engine {
	GlobalShader::GlobalShader(const std::filesystem::path& filePath, const std::string& entryPoint) : filePath(filePath), entryPoint(entryPoint) {};
	
	void GlobalShader::Create(rhi::Device* Device) {
		shaderModule = renderInterface->InitShader();
		shaderModule->Create(Device, filePath);
	}
	void GlobalShader::Destroy(rhi::Device* Device) {
		renderInterface->DestroyShader(shaderModule, Device);
	}
}