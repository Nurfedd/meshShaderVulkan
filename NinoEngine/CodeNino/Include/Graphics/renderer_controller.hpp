#pragma once
#include "scene_renderer.hpp"
#include "Interface/texture.hpp"
#include "Interface/device.hpp"
#include "Interface/command_buffer.hpp"
#include "Interface/dynamic_renderer.hpp"
#include "engine_graphic_resources.hpp"
#include "service_locator.hpp"
#include <functional>
#include <vector>
namespace nino_engine {
	// this class is used to renderer multiple scene render context
	struct SceneRenderContext {
		rhi::Texture* texture = nullptr;
		std::unique_ptr<SceneRenderer> sceneRenderer;
		bool renderOnSwapchain = true;
	};

	class RendererController {
	public :
		void Create();

		bool BeginFrame();
		void RenderFrame();
		void AddDrawDataFunc(std::function<void(rhi::CommandBuffer*)> func);
		void Destroy();
		template <typename T>
		SceneRenderContext* CreateNewRenderContext(rhi::Texture* textureToDrawOn, bool renderOnSwapchain) {
			if (textureToDrawOn == nullptr && !renderOnSwapchain)
				return nullptr;

			static_assert(std::is_base_of<SceneRenderer, T>::value);
			rhi::Device* device = ServiceLocator::Get<EngineGraphicResources>()->Device;
			std::unique_ptr<T> newRenderer = std::make_unique<T>();
			newRenderer->Create(device);

			std::unique_ptr<SceneRenderContext> sceneRenderContext = std::make_unique<SceneRenderContext>();
			sceneRenderContext->sceneRenderer = std::move(newRenderer);
			sceneRenderContext->texture = textureToDrawOn;
			sceneRenderContext->renderOnSwapchain = renderOnSwapchain;

			SceneRenderContext* rawSceneRenderContext = sceneRenderContext.get();
			renderContexts.push_back(std::move(sceneRenderContext));

			return rawSceneRenderContext;
		};
	private :

		void PresentSwapchain();

		rhi::DeviceQueue* graphicQueue = nullptr;
		rhi::DeviceQueue* presentQueue = nullptr;

		std::vector<rhi::CommandBuffer*> graphicCommandBuffers;
		std::vector<std::unique_ptr<SceneRenderContext>> renderContexts;

		rhi::DynamicRenderer* additionalSwapchainRenderer = nullptr;
		std::function<void(rhi::CommandBuffer*)> onCommandRecorded;
	};
}