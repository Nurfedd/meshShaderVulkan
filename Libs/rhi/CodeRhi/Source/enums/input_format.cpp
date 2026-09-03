#include "enums/input_fomat.hpp"

namespace rhi {
	size_t GetSizeFromInputFormat(InputFormat inputFormat)
	{
		switch (inputFormat)
		{
		case rhi::FLOAT1:   return sizeof(float);
		case rhi::FLOAT2:   return sizeof(float) * 2;
		case rhi::FLOAT3:   return sizeof(float) * 3;
		case rhi::FLOAT4:   return sizeof(float) * 4;

		case rhi::INT1:     return sizeof(int32_t);
		case rhi::INT2:     return sizeof(int32_t) * 2;
		case rhi::INT3:     return sizeof(int32_t) * 3;
		case rhi::INT4:     return sizeof(int32_t) * 4;

		case rhi::UINT32_1: return sizeof(uint32_t);
		case rhi::UINT32_2: return sizeof(uint32_t) * 2;
		case rhi::UINT32_3: return sizeof(uint32_t) * 3;
		case rhi::UINT32_4: return sizeof(uint32_t) * 4;

		case rhi::UINT64_1: return sizeof(uint64_t);
		case rhi::UINT64_2: return sizeof(uint64_t) * 2;
		case rhi::UINT64_4: return sizeof(uint64_t) * 4;

		default:
			return 0;
		}
	}
}