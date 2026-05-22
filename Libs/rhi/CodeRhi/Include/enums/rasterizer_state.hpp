#pragma once
namespace rhi {
	enum PolygonMode {
		FILL,
		LINE,
		POINT
	};
	enum FrontFace {
		CLOCKWISE,
		COUNTER_CLOCKWISE
	};
	enum CullModeFlags {
		CULL_NONE = 0,
		BACK_FACE = 1 << 1,
		FRONT_FACE = 1 << 2,
		CULL_ALL = 0x7FFFFFFF,
	};
}