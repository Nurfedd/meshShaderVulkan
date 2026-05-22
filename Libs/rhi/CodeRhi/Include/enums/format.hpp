#pragma once

namespace rhi {
	enum PresentMode {
		IMMEDIATE_MODE,
		MAILBOX_MODE,
		FIFO_MODE
	};

	enum Format {
		Undefined = 0,
		RGBA16_SFLOAT,
		R8_UNORM,
		RG8_UNORM,
		RGB8_UNORM,
		RGBA8_UNORM,
		RGBA16_UNORM,
		R8_SRGB,
		RG8_SRGB,
		RGB8_SRGB,
		RGBA8_SRGB,
		BGRA8_UNORM,
		BGRA8_SRGB,

		D32_FLOAT,
		D24_UNORM_S8_UINT,
	};
	enum DepthOperator {
		OP_NEVER = 0,
		OP_LESS = 1,
		OP_EQUAL = 2,
		OP_LESS_OR_EQUAL = 3,
		OP_GREATER = 4,
		OP_NOT_EQUAL = 5,
		OP_GREATER_OR_EQUAL = 6,
		OP_ALWAYS = 7,
	};
	enum ColorSpace {
		SRGB_NON_LINEAR,
	};
}