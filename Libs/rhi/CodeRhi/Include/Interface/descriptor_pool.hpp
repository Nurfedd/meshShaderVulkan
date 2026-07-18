#pragma once
#include "device.hpp"
#include "set_layout.hpp"
#include "descriptor_set.hpp"
namespace rhi {
	class VulkanDescriptorPool;
	class DescriptorPool {
	public :
		virtual ~DescriptorPool() = default;
		virtual void Create(Device* device, SetLayout** layouts, uint32_t layoutCount,uint32_t maxSets) = 0;
		virtual void AllocateSets(Device* device, DescriptorSet** sets,SetLayout** layouts,uint32_t setCount,uint32_t* variableCounts) = 0;
		virtual void Reset(Device* device) = 0;
		virtual void Destroy(Device* device) = 0;
		virtual VulkanDescriptorPool& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanDescriptorPool"); }
	


	};
}