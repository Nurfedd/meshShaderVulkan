#include "IVulkan/vulkan_shader.hpp"
#include "IVulkan/vulkan_device.hpp"
#include <fstream>
#include "Volk/volk.h"
namespace rhi {
	bool VulkanShader::Create(Device* device, std::filesystem::path path) {
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		std::ifstream file(path, std::ios::ate | std::ios::binary);
		std::vector<char> buffer;
		if (file.is_open()) {
			size_t fileSize = (size_t)file.tellg();

			buffer.resize(fileSize);
			file.seekg(0);
			file.read(buffer.data(), fileSize);
			file.close();

			VkShaderModuleCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());
			createInfo.codeSize = fileSize;
			if (vkCreateShaderModule(vulkanDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create Vulkan shader module");
				return false;
			}
			return true;
		}
		return false;
	}
	void VulkanShader::Destroy(Device* device) {
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		vkDestroyShaderModule(vulkanDevice, shaderModule, nullptr);
	}
}