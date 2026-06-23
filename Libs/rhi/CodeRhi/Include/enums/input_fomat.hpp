#pragma once
#include "nino_core.hpp"
namespace rhi {
	enum InputFormat {
		FLOAT1,
		FLOAT2,
		FLOAT3,
		FLOAT4,

		INT1,
		INT2,
		INT3,
		INT4,

		UINT32_1,
		UINT32_2,
		UINT32_3,
		UINT32_4,

		UINT64_1,
		UINT64_2,
		UINT64_4,
	};

	enum InputRate {
		VERTEX_RATE,
		INSTANCE_RATE
	};
	enum DescriptorType {
		DESCRIPTOR_UNIFORM_BUFFER,
		DESCRIPTOR_STORAGE_BUFFER,
		DESCRIPTOR_SAMPLED_TEXTURE,
	};
	enum ShaderStagesFlags {
		VERTEX_STAGE = 1 << 1,
		FRAGMENT_STAGE = 1 << 2,
		COMPUTE_STAGE = 1 << 3,
	};
	ENUM_CLASS_FLAGS(ShaderStagesFlags);
	enum IndexType {
		INDEX_TYPE_UINT_8,
		INDEX_TYPE_UINT_16,
		INDEX_TYPE_UINT_32,
	};
}