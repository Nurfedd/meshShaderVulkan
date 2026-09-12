#include "Graphics/SceneRenderers/triangle_renderer.hpp"
#include "Interface/graphic_api.hpp"
using namespace rhi;
namespace nino_engine {
	
	void TriangleRenderer::Create(Device* device) {
		SceneRenderer::Create(device);

		triangleVertexShader = renderInterface->InitShader();
		triangleFragmentShader = renderInterface->InitShader();

		bool compiled = triangleVertexShader->Create(device, "Resources/Spv/triangle.vert.spv") && triangleFragmentShader->Create(device, "Resources/Spv/triangle.frag.spv");
		if (!compiled)
			throw std::runtime_error("failed to compiled triangle shaders");

		trianglePipeline = renderInterface->InitGraphicPipeline();
		
		ColorAttachmentDescriptor colorAttachment;
		colorAttachment.enableBlending = false;
		colorAttachment.format = RGBA8_UNORM;

		RasterizerInfo rasterizerInfo;
		rasterizerInfo.attachmentCount = 1;
		rasterizerInfo.colorAttachments = &colorAttachment;
		rasterizerInfo.depthWrite = false;
		rasterizerInfo.depthTestEnable = false;
		rasterizerInfo.cullModeFlags = CULL_NONE;
		rasterizerInfo.frontFace = COUNTER_CLOCKWISE;
		rasterizerInfo.polygonMode = FILL;
		rasterizerInfo.depthOperator = OP_LESS;
		

		GraphicVertexPipelineCreateInfo pipelineCreateInfo;
		pipelineCreateInfo.fragmentShader = triangleFragmentShader;
		pipelineCreateInfo.vertexShader = triangleVertexShader;
		pipelineCreateInfo.rasterizerInfo = rasterizerInfo;

		trianglePipeline->CreateVertexPipeline(device, pipelineCreateInfo);

		triangleRenderer = renderInterface->InitDynamicRenderer();
		
	}
	SceneRenderedInfo TriangleRenderer::RenderFrame(Texture* texture) {
		SceneRenderedInfo renderedInfo;
		
		CommandBuffer* commandBuffer = GetCurrentCommandBuffer();
		renderedInfo.commandToSubmit = commandBuffer;

		uint32_t width = texture->GetWidth();
		uint32_t height = texture->GetHeight();

		commandBuffer->Reset();
		commandBuffer->Begin();

		commandBuffer->SetViewport(0, 0, width, height);
		commandBuffer->SetScissor(0, 0, width, height);

		Attachment colorAttachment;
		colorAttachment.clearBuffer = true;
		colorAttachment.clearValue = ClearValue(0, 0, 0, 0);
		colorAttachment.texture = texture;

		BeginRenderingInfo renderingInfo;
		renderingInfo.colorAttachmentCount = 1;
		renderingInfo.colorAttachments = &colorAttachment;
		renderingInfo.height = height;
		renderingInfo.width = width;
		renderingInfo.depthAttachment = nullptr;

		triangleRenderer->Begin(commandBuffer,renderingInfo);

		trianglePipeline->Bind(commandBuffer);
		commandBuffer->Draw(3, 1, 0, 0);

		triangleRenderer->End(commandBuffer);
		commandBuffer->End();
		return renderedInfo;
	}
	void TriangleRenderer::Destroy(Device* device) {
		SceneRenderer::Destroy(device);
		renderInterface->DestroyShader(triangleFragmentShader, device);
		renderInterface->DestroyShader(triangleVertexShader, device);
		renderInterface->DestroyPipeline(trianglePipeline, device);
		renderInterface->DestroyDynamicRenderer(triangleRenderer);
	}
}