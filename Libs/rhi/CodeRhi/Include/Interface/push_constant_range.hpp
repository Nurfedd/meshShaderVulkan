#pragma once
#include "enums/input_fomat.hpp"
#include <stdint.h>
#include <exception>

namespace rhi {
	struct PushConstantRange {
	public :
		PushConstantRange(size_t size,size_t offset,ShaderStagesFlags stages) : size(size),offset(offset),stages(stages) {}
		size_t size;
		size_t offset;
		ShaderStagesFlags stages;
	
	};
}