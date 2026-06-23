#pragma once
#include "nino_core.hpp"
namespace rhi {
    enum PipelineStageFlags {
        PIPELINE_STAGE_NONE = 0,
        PIPELINE_STAGE_TOP_OF_PIPE = 1 << 0,
        PIPELINE_STAGE_DRAW_INDIRECT = 1 << 1,
        PIPELINE_STAGE_VERTEX_INPUT = 1 << 2,
        PIPELINE_STAGE_VERTEX_SHADER = 1 << 3,
        PIPELINE_STAGE_FRAGMENT_SHADER = 1 << 4,
        PIPELINE_STAGE_COMPUTE_SHADER = 1 << 5,
        PIPELINE_STAGE_TRANSFER = 1 << 6,
        PIPELINE_STAGE_BOTTOM_OF_PIPE = 1 << 7,
        PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT = 1 << 8,
        PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT = 1 << 9,
        PIPELINE_STAGE_ALL_COMMANDS = 1 << 10,
	};
    ENUM_CLASS_FLAGS(PipelineStageFlags);
    enum AccessFlags
    {
        NONE_ACCESS = 0,
        TRANSFER_READ = 1 << 1,
        TRANSFER_WRITE = 1 << 2,
        SHADER_READ = 1 << 3,
        SHADER_WRITE = 1 << 4,
        VERTEX_READ = 1 << 5,
        INDEX_READ = 1 << 6,
        UNIFORM_READ = 1 << 7,
        COLOR_ATTACHMENT_WRITE = 1 << 8,
        COLOR_ATTACHMENT_READ = 1 << 9,
        DEPTH_STENCIL_WRITE = 1 << 10,
    };
    ENUM_CLASS_FLAGS(AccessFlags);
    enum ImageLayout
    {
        UNDEFINED = 0,
        GENERAL,
        COLOR_ATTACHMENT,
        DEPTH_STENCIL_ATTACHMENT,
        DEPTH_STENCIL_READ_ONLY,
        SHADER_READ_ONLY,
        TRANSFER_SRC,
        TRANSFER_DST,
        PRESENT
    };
}