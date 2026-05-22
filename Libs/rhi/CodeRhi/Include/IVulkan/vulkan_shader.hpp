#pragma once
#include "Interface/shader.hpp"
#include "vulkan/vulkan.h"
namespace rhi {
	class VulkanShader : public Shader {
	public :
		bool Create(Device* device, std::filesystem::path path) override;
		void Destroy(Device* device) override;
		VulkanShader& API_VULKAN() override { return *(this); }
		VkShaderModule GetModule() const { return shaderModule; }
	private :
		VkShaderModule shaderModule = VK_NULL_HANDLE;
	};
}