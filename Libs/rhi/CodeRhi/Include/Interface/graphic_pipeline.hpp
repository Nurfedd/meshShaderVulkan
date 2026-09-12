#pragma once
#include "Interface/pipeline.hpp"
#include "enums/rasterizer_state.hpp"
#include "enums/format.hpp"
#include "Interface/vertex_layout.hpp"
#include "Interface/set_layout.hpp"
#include "Interface/push_constant_range.hpp"
#include "Interface/shader.hpp"

namespace rhi {
	class VulkanGraphicPipeline;

	struct ColorAttachmentDescriptor {
		Format format;
		bool enableBlending = false;
	};

	struct RasterizerInfo {
		PolygonMode polygonMode;
		FrontFace frontFace;
		CullModeFlags cullModeFlags;
		Format depthFormat = Undefined;
		ColorAttachmentDescriptor* colorAttachments;
		uint32_t attachmentCount;
		bool depthTestEnable;
		bool depthWrite;
		DepthOperator depthOperator;
	};

	struct GraphicPipelineCreateInfo {
		PushConstantRange* pushConstants = nullptr;
		SetLayout** setLayouts = nullptr;
		VertexLayout* vertexLayouts = nullptr;


		uint32_t pushConstantCount = 0;
		uint32_t setLayoutCount = 0;
		uint32_t vertexLayoutCount = 0;

		RasterizerInfo rasterizerInfo;
	};
	struct GraphicVertexPipelineCreateInfo : public GraphicPipelineCreateInfo {
		Shader* vertexShader = nullptr;
		Shader* fragmentShader = nullptr;
		Shader* geometryShader = nullptr;
	};

	struct GraphicMeshPipelineCreateInfo : public GraphicPipelineCreateInfo {
		Shader* fragmentShader = nullptr;
		Shader* taskShader = nullptr;
		Shader* meshShader = nullptr;
	};

	class GraphicPipeline : public Pipeline {
	public :
		virtual ~GraphicPipeline() = default;
		virtual void CreateVertexPipeline(Device* device, const GraphicVertexPipelineCreateInfo& graphicVertexPipelineCreateInfo) = 0;
		virtual void CreateMeshPipeline(Device* device, const GraphicMeshPipelineCreateInfo& graphicMeshPipelineCreateInfo) = 0;
		virtual VulkanGraphicPipeline& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanGraphicPipeline"); }
	protected :
	};
}