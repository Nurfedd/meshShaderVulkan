#pragma once
#include "Graphics/scene_renderer.hpp"
#include "Interface/graphic_pipeline.hpp"
#include "Interface/dynamic_renderer.hpp"
namespace nino_engine {
	class TriangleRenderer : public SceneRenderer {
	public :
		void Create(rhi::Device* device) override;
		SceneRenderedInfo RenderFrame(rhi::Texture* texture) override;
		void Destroy(rhi::Device* device) override;
	protected :

	private :
		rhi::DynamicRenderer* triangleRenderer = nullptr;
		rhi::GraphicPipeline* trianglePipeline = nullptr;
		rhi::Shader* triangleVertexShader = nullptr;
		rhi::Shader* triangleFragmentShader = nullptr;
	};
}