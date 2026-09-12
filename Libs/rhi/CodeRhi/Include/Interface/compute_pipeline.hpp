#pragma once
#include "device.hpp"
#include "pipeline.hpp"
#include "set_layout.hpp"
#include "push_constant_range.hpp"
#include "shader.hpp"

namespace rhi {
	class VulkanComputePipeline;

	struct ComputePipelineCreateInfo {
		Shader* computeShader;
		SetLayout** setLayouts;
		PushConstantRange* pushConstants;

		uint32_t setLayoutCount;
		uint32_t pushConstantCount;
	};

	class ComputePipeline : public Pipeline {
	public :
		virtual ~ComputePipeline() = default;
		virtual void Create(Device* device, ComputePipelineCreateInfo createInfo) = 0;
		virtual VulkanComputePipeline& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanComputePipeline"); }
	};
}