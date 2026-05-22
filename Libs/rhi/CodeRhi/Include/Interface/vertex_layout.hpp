#pragma once
#include <stdint.h>
#include "enums/input_fomat.hpp"
#include <exception>
namespace rhi {
	class VulkanVertexLayout;
	struct VertexInput {
		VertexInput() = default;
		VertexInput(uint32_t _location, uint32_t _offset, InputFormat _inputFormat) { location = _location; offset = _offset; inputFormat = _inputFormat; };
		uint32_t location;
		uint32_t offset = 0;
		InputFormat inputFormat;
	};
	class VertexLayout {
	public :
		virtual ~VertexLayout() = default;
		virtual void Create(VertexInput* vertexInputs, uint32_t inputCount, uint32_t _binding, uint32_t _stride) { binding = _binding; stride = _stride; };
		virtual VulkanVertexLayout& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanVertexLayout"); }
	protected :
		uint32_t binding;
		uint32_t stride;
	};
}