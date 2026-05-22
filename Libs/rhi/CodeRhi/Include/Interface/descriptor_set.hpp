#pragma once
#include <exception>
#include "buffer.hpp"
#include "device.hpp"
#include "texture.hpp"
#include "sampler.hpp"
#include "enums/input_fomat.hpp"

namespace rhi {
	class VulkanDescriptorSet;
	struct BufferUpdateInfo {
		Buffer** buffers;
		uint32_t count = 1;
		uint32_t dstArrayElement = 0;
	};
	class DescriptorSet {
	public :
		virtual ~DescriptorSet() = default;
		virtual void UpdateBuffer(Device* device, uint32_t binding, BufferUpdateInfo bufferUpdateInfo, DescriptorType descriptorType) = 0;
		virtual void UpdateTexture(Device* device, uint32_t binding, Texture* texture, Sampler* sampler, uint32_t dstArrayElement) = 0;
		virtual VulkanDescriptorSet& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanDescriptorSet"); };
	};
}