#pragma once
#include "device.hpp"
#include "enums/image_type.hpp"
namespace rhi {
	class VulkanSampler;
	struct SamplerCreateInfo {
		SamplerFilter filter;
		SamplerAdressMode adressModeU;
		SamplerAdressMode adressModeV;
		SamplerAdressMode adressModeW;
		int maxMipLevel;
		bool compareEnable;
		bool useAnisotropy;
		bool useMipmap;
	};
	class Sampler {
	public :
		virtual ~Sampler() = default;
		virtual void Create(Device* device, SamplerCreateInfo createInfo) = 0;
		virtual void Destroy(Device* device) = 0;
		virtual VulkanSampler& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanSampler"); }
	};
}