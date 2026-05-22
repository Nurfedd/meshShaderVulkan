#pragma once
#include "device.hpp"
#include "command_buffer.hpp"
#include "enums/input_fomat.hpp"
#include "descriptor_set.hpp"
namespace rhi {
	class Pipeline {
	public :
		virtual ~Pipeline() = default;
		virtual void PushConstant(CommandBuffer* commandBuffer, uint32_t offset, size_t size, void* data, ShaderStagesFlags shaderStages) = 0;
		virtual void Bind(CommandBuffer* commandBuffer) = 0;
		virtual void Destroy(Device* device) = 0;
		virtual void BindDescriptorSet(CommandBuffer* commandBuffer, DescriptorSet** sets, uint32_t setCount, uint32_t firstSet) = 0;
	protected :

	};
}