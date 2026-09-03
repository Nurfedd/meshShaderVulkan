#pragma once
#include "scene_renderer.hpp"
#include "Interface/texture.hpp"
#include "Interface/device.hpp"
#include "Interface/command_buffer.hpp"
#include "Interface/dynamic_renderer.hpp"
#include "engine_graphic_resources.hpp"
#include "service_locator.hpp"
#include "frame_data.hpp"
#include "task.h"
#include <functional>
#include <queue>
#include <vector>

DECLARE_DELEGATE(DOnPrevBeginFrame);
DECLARE_DELEGATE(DOnPostBeginFrame);
DECLARE_DELEGATE(DOnPostRenderFrame);
DECLARE_DELEGATE(DOnCommandBufferRecorded, rhi::CommandBuffer*)
DECLARE_DELEGATE(DOnDestroy)

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
		void PushFrame(FrameData frameData);
		void Destroy();
		
		uint64_t GetFrameCount();
		template <typename T>
		SceneRenderContext* CreateNewRenderContext(rhi::Texture* textureToDrawOn, bool renderOnSwapchain) {
			if (textureToDrawOn == nullptr && !renderOnSwapchain)
				return nullptr;

			static_assert(std::is_base_of<SceneRenderer, T>::value);
			
			std::unique_ptr<T> newRenderer = std::make_unique<T>();
			newRenderer->Create(Device);

			std::unique_ptr<SceneRenderContext> sceneRenderContext = std::make_unique<SceneRenderContext>();
			sceneRenderContext->sceneRenderer = std::move(newRenderer);
			sceneRenderContext->texture = textureToDrawOn;
			sceneRenderContext->renderOnSwapchain = renderOnSwapchain;

			SceneRenderContext* rawSceneRenderContext = sceneRenderContext.get();
			renderContexts.push_back(std::move(sceneRenderContext));

			return rawSceneRenderContext;
		};
		DOnCommandBufferRecorded OnCommandBufferRecorded;

		DOnDestroy OnBeginDestroy;
		DOnPrevBeginFrame OnPrevBeginFrame;
		DOnPostBeginFrame OnPostBeginFrame;
		DOnPostRenderFrame OnPostRenderFrame;
	private :
		bool BeginFrame();
		void RenderFrame();
		void PresentSwapchain();

		void WaitRender();
		void RenderTaskAsync();
		bool shouldStop = false;

		std::mutex frameDataMutex;
		std::condition_variable frameDataCV_notFull;
		std::condition_variable frameDataCV_notEmpty;
		std::queue<FrameData> frameDatas;

		std::shared_ptr<mt::Task> lastRenderTask;

		rhi::Device* Device = nullptr;
		rhi::DeviceQueue* graphicQueue = nullptr;
		rhi::DeviceQueue* presentQueue = nullptr;

		std::vector<rhi::CommandBuffer*> graphicCommandBuffers;
		std::vector<std::unique_ptr<SceneRenderContext>> renderContexts;

		rhi::DynamicRenderer* additionalSwapchainRenderer = nullptr;
		uint64_t frameCount = 0;
	};
}