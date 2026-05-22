#include "IVulkan/vulkan_sampler.hpp"
#include "IVulkan/enums/vulkan_image_type.hpp"
#include "Volk/volk.h"
namespace rhi {
	SamplerCreateInfoVulkan::SamplerCreateInfoVulkan(SamplerCreateInfo createInfo) {
		filter = ToVulkanFilter(createInfo.filter);
		adressModeU = ToVulkanAdressMode(createInfo.adressModeU);
		adressModeV = ToVulkanAdressMode(createInfo.adressModeV);
		adressModeW = ToVulkanAdressMode(createInfo.adressModeW);
		maxMipLevel = createInfo.maxMipLevel;
		compareEnable = createInfo.compareEnable;
		useMipmap = createInfo.useMipmap;
		useAnisotropy = createInfo.useAnisotropy;
	}
	void VulkanSampler::CreateVk(VulkanDevice& vulkanDevice, SamplerCreateInfoVulkan createInfoVulkan) {
		VkSamplerCreateInfo samplerCI{};
		VulkanPhysicalDevice& vulkanPhysicalDevice = vulkanDevice.GetPhysicalDevice()->API_VULKAN();
		samplerCI.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		
		samplerCI.magFilter = createInfoVulkan.filter;
		samplerCI.minFilter = createInfoVulkan.filter;

		samplerCI.addressModeU = createInfoVulkan.adressModeU;
		samplerCI.addressModeV = createInfoVulkan.adressModeV;
		samplerCI.addressModeW = createInfoVulkan.adressModeW;

		samplerCI.minLod = 0.0f;
		samplerCI.mipLodBias = 0.0f;

		if (createInfoVulkan.useMipmap) {
			samplerCI.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			samplerCI.maxLod = createInfoVulkan.maxMipLevel;
		}
		else {
			samplerCI.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
			samplerCI.maxLod = 0;
		}

		if (createInfoVulkan.useAnisotropy) {
			float maxDeviceAnisotropy = vulkanPhysicalDevice.GetLimits().limits.maxSamplerAnisotropy;
			samplerCI.maxAnisotropy = maxDeviceAnisotropy;
			samplerCI.anisotropyEnable = VK_TRUE;
		}
		else {
			samplerCI.maxAnisotropy = 0;
			samplerCI.anisotropyEnable = VK_FALSE;
		}

		samplerCI.compareEnable = createInfoVulkan.compareEnable;
		samplerCI.compareOp = VK_COMPARE_OP_LESS;
		
		vkCreateSampler(vulkanDevice, &samplerCI, nullptr, &sampler);
	}
	void VulkanSampler::DestroyVk(VulkanDevice& vulkanDevice) {
		vkDestroySampler(vulkanDevice, sampler, nullptr);
	}

	void VulkanSampler::Create(Device* device, SamplerCreateInfo createInfo) {
		SamplerCreateInfoVulkan createInfoVulkan = createInfo;
		CreateVk(device->API_VULKAN(), createInfoVulkan);
	}

	void VulkanSampler::Destroy(Device* device) {
		DestroyVk(device->API_VULKAN());
	}
}