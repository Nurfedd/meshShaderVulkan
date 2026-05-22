#pragma once
#include "Interface/device.hpp"
#include <filesystem>
namespace rhi {
	class VulkanShader;
	class Shader {
	public :
		virtual ~Shader() = default;
		virtual bool Create(Device* device, std::filesystem::path filePath) = 0;
		virtual void Destroy(Device* device) = 0;
		virtual VulkanShader& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanShader"); }
	private :

	};
}