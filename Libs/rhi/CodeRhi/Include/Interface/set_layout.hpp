#pragma once
#include <stdint.h>
#include "enums/input_fomat.hpp"
#include "device.hpp"
namespace rhi {
	class VulkanSetLayout;

	struct SetInfo {
		SetInfo() = default;
		SetInfo(uint32_t _binding, uint32_t _count, int stages, DescriptorType type) {
			binding = _binding;
			count = _count;
			shaderStages = static_cast<ShaderStagesFlags>(stages);
			descriptorType = type;
		};
		uint32_t binding;
		uint32_t count = 1;
		ShaderStagesFlags shaderStages;
		DescriptorType descriptorType;
	};
	class SetLayout {
	public :
		virtual ~SetLayout() = default;
		virtual void Create(Device* device, SetInfo* setInfos, uint32_t setInfoCount, bool* variable);
		virtual void Destroy(Device* device) = 0;
		virtual VulkanSetLayout& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanSetLayout"); }
		std::unordered_map<DescriptorType, uint32_t> GetLayoutInfo() const { return layoutInfos; }
	protected :
		std::unordered_map<DescriptorType, uint32_t> layoutInfos;
	};
}