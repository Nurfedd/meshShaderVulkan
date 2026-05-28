#pragma once
#include "GLFW/glfw3.h"
#include "scene_renderer.hpp"
#include "Interface/texture.hpp"
#include "Interface/device.hpp"
#include "engine_graphic_resources.hpp"
#include "service_locator.hpp"
#include <memory>
namespace msvk {
	struct SceneRenderContext {
		rhi::Texture* texture = nullptr;
		std::unique_ptr<SceneRenderer> sceneRenderer;
		bool renderOnSwapchain = true;
	};
	class Engine {
	public :
		void Init(GLFWwindow* window);
		
		void Update();
		void Destroy();

		template <typename T> 
		SceneRenderContext* CreateNewRenderContext(rhi::Texture* textureToDrawOn,bool renderOnSwapchain) {
			if (textureToDrawOn == nullptr && !renderOnSwapchain)
				return nullptr;

			static_assert(std::is_base_of<SceneRenderer, T>::value);
			rhi::Device* device = ServiceLocator::Get<EngineGraphicResources>()->GetDevice();
			std::unique_ptr<T> newRenderer = std::make_unique<T>();
			newRenderer->Create(device);

			std::unique_ptr<SceneRenderContext> sceneRenderContext = std::make_unique<SceneRenderContext>();
			sceneRenderContext->sceneRenderer = std::move(newRenderer);
			sceneRenderContext->texture = textureToDrawOn;
			sceneRenderContext->renderOnSwapchain = renderOnSwapchain;

			SceneRenderContext* rawSceneRenderContext = sceneRenderContext.get();
			renderContexts.push_back(std::move(sceneRenderContext));

			return rawSceneRenderContext;
		}
	private :
		float lastTime = 0.f;
		float deltaTime = 0.f;

		rhi::DeviceQueue* graphicQueue;
		rhi::DeviceQueue* presentQueue;

		std::vector<rhi::CommandBuffer*> graphicCommandBuffers;
		std::vector<std::unique_ptr<SceneRenderContext>> renderContexts;

		bool BeginFrame();
		void RenderFrame();
		void PresentSwapchain();
	};
}