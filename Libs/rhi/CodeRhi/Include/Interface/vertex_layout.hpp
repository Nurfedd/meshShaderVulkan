#pragma once
#include <stdint.h>
#include "enums/input_fomat.hpp"
#include <exception>
namespace rhi {
	
	struct VertexInput {
		VertexInput() = default;
		VertexInput(uint32_t _location, uint32_t _offset, InputFormat _inputFormat) { location = _location; offset = _offset; inputFormat = _inputFormat; };
		uint32_t location;
		uint32_t offset = 0;
		InputFormat inputFormat;
	};
	struct VertexLayout {
	public :
		VertexLayout(VertexInput* vertexInputs, uint32_t inputCount, uint32_t binding, uint32_t stride) :
		vertexInputs(vertexInputs),inputCount(inputCount), binding(binding), stride(stride) 
		{}
		VertexInput* vertexInputs; 
		uint32_t inputCount; 
		uint32_t binding; 
		uint32_t stride;
	};
}