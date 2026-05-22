#pragma once
#include "vulkan/vulkan.h"
#include "Interface/sampler.hpp"
#include "IVulkan/vulkan_device.hpp"
namespace rhi {
	struct SamplerCreateInfoVulkan {
		SamplerCreateInfoVulkan() = default;
		SamplerCreateInfoVulkan(SamplerCreateInfo createInfo);
		VkFilter filter;
		VkSamplerAddressMode adressModeU;
		VkSamplerAddressMode adressModeV;
		VkSamplerAddressMode adressModeW;
		int maxMipLevel;
		bool compareEnable;
		bool useAnisotropy;
		bool useMipmap;
	};
	class VulkanSampler : public Sampler {
	public :
		void CreateVk(VulkanDevice& vulkanDevice, SamplerCreateInfoVulkan createInfoVulkan);
		void Create(Device* device, SamplerCreateInfo createInfo) override;
		void DestroyVk(VulkanDevice& vulkanDevice);
		VkSampler GetSampler() { return sampler; }
		void Destroy(Device* device) override;
		VulkanSampler& API_VULKAN() override { return *(this); }
	private :
		VkSampler sampler = VK_NULL_HANDLE;
	};
}