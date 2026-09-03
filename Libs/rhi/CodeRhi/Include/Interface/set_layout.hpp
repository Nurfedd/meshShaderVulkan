#pragma once
#include <stdint.h>
#include "enums/input_fomat.hpp"
#include "device.hpp"
namespace rhi {
	class VulkanSetLayout;

	struct SetInfo {
		SetInfo() = default;
		SetInfo(uint32_t _binding, uint32_t _count, ShaderStagesFlags stages, DescriptorType type,bool isVariable = false) {
			binding = _binding;
			count = _count;
			shaderStages = stages;
			descriptorType = type;
			variable = isVariable;
		};
		uint32_t binding;
		uint32_t count = 1;
		ShaderStagesFlags shaderStages;
		DescriptorType descriptorType;
		bool variable;
	};
	class SetLayout {
	public :
		virtual ~SetLayout() = default;
		virtual void Create(Device* device, SetInfo* setInfos, uint32_t setInfoCount);
		virtual void Destroy(Device* device) = 0;
		virtual VulkanSetLayout& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanSetLayout"); }
		std::unordered_map<DescriptorType, uint32_t> GetLayoutInfo() const { return layoutInfos; }
	protected :
		std::unordered_map<DescriptorType, uint32_t> layoutInfos;
	};
}