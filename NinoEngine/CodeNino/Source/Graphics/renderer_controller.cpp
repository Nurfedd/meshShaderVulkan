#include "Graphics/renderer_controller.hpp"
#include "Interface/graphic_api.hpp"
#include "Graphics/engine_graphic_resources.hpp"
#include "task_manager.hpp"
#include "service_locator.hpp"
using namespace rhi;
namespace nino_engine {
	void RendererController::Create() {
		EngineGraphicResources* engineResources = ServiceLocator::Get<EngineGraphicResources>();
		uint32_t frameCount = engineResources->GetFrameInFlightCount();
		Device* device = engineResources->Device;
		
		graphicQueue = engineResources->GraphicQueue;
		presentQueue = engineResources->PresentQueue;

		CommandPool* commandPool = engineResources->GraphicCommandPool;
		graphicCommandBuffers.resize(frameCount);
		for (uint32_t i = 0; i < frameCount; i++) {
			CommandBuffer* newCommandBuffer = renderInterface->InitCommandBuffer();
			graphicCommandBuffers[i] = newCommandBuffer;
		}
		commandPool->AllocateCommandBuffers(device, graphicCommandBuffers.data(), graphicCommandBuffers.size());

		additionalSwapchainRenderer = renderInterface->InitDynamicRenderer();

		TaskManager* taskManager = ServiceLocator::Get<TaskManager>();
		lastRenderTask = taskManager->AsyncTask(TaskType::Render_Task, std::bind(&RendererController::RenderTaskAsync, this));

	}
	
	void RendererController::PushFrame(FrameData frameData) {
		std::unique_lock lock(frameDataMutex);
		EngineGraphicResources* engineResources = ServiceLocator::Get<EngineGraphicResources>();
		uint32_t frameCount = engineResources->GetFrameInFlightCount();
		
		frameDataCV_notFull.wait(lock, [this,frameCount] {
			return frameDatas.size() < frameCount || shouldStop;
			});

		if (shouldStop) return;

		frameDatas.push(std::move(frameData));

		lock.unlock();
		frameDataCV_notEmpty.notify_one();
	}

	// check if any frame data available
	void RendererController::RenderTaskAsync() {
		while (!shouldStop)
		{
			std::unique_lock lock(frameDataMutex);
			frameDataCV_notEmpty.wait(lock, [this]() {
				return !frameDatas.empty() || shouldStop;
				});

			if (shouldStop) {
				return;
			}
			frameDatas.pop();
			lock.unlock();
			frameDataCV_notFull.notify_one();
			OnPrevBeginFrame.Broadcast();
			if (BeginFrame()) {
				OnPostBeginFrame.Broadcast();
				RenderFrame();
				OnPostRenderFrame.Broadcast();
			}
		}
	}
	void RendererController::WaitRender() {
		if (lastRenderTask) {
			lastRenderTask->Wait();
		}
	}

	bool RendererController::BeginFrame() {
		EngineGraphicResources* engineResources = ServiceLocator::Get<EngineGraphicResources>();
		Device* device = engineResources->Device;

		Fence* currentFence = engineResources->GetCurrentFence();
		Semaphore* imageAvailableSemaphore = engineResources->GetImageAvailableSemaphore(engineResources->GetCurrentFrame());
		Swapchain* swapchain = engineResources->Swapchain;

		device->WaitForFences(&currentFence, 1, true);

		SwapchainAcquireResult acquireResult = swapchain->AcquireImage(device, nullptr, imageAvailableSemaphore);


		if (acquireResult.success) {
			device->ResetFences(&currentFence, 1);
		}
		else {
			device->WaitIdle();
			swapchain->Recreate(device);
		}
		return acquireResult.success;
	}
	
	void RendererController::RenderFrame() {
		EngineGraphicResources* engineResources = ServiceLocator::Get<EngineGraphicResources>();
		Device* device = engineResources->Device;
		Swapchain* swapchain = engineResources->Swapchain;

		uint32_t imageIndex = swapchain->GetImageIndex();
		uint32_t currentFrame = engineResources->GetCurrentFrame();


		Texture* swapchainTexture = swapchain->GetTexture(imageIndex);

		Fence* currentFence = engineResources->GetCurrentFence();
		Semaphore* imageAvailableSemaphore = engineResources->GetImageAvailableSemaphore(engineResources->GetCurrentFrame());
		Semaphore* imageRendererSemaphore = engineResources->GetImageRenderedSemaphore(imageIndex);


		CommandBuffer* commandBuffer = graphicCommandBuffers[currentFrame];

		std::vector<Semaphore*> waitSemaphores(1);
		std::vector<PipelineStageFlags> waitStages(1);
		std::vector<CommandBuffer*> renderCommands;
		std::vector<uint64_t> waitValues;

		waitSemaphores[0] = imageAvailableSemaphore;
		waitStages[0] = PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT;


		commandBuffer->Reset();
		commandBuffer->Begin();

		for (std::unique_ptr<SceneRenderContext>& sceneRenderContext : renderContexts) {
			SceneRenderer* sceneRenderer = sceneRenderContext->sceneRenderer.get();
			Texture* texture = (sceneRenderContext->renderOnSwapchain) ? swapchainTexture : sceneRenderContext->texture;
			SceneRenderedInfo renderedInfo = sceneRenderer->RenderFrame(texture);

			if (renderedInfo.commandToSubmit) {
				renderCommands.push_back(renderedInfo.commandToSubmit);
			}
			for (SemaphoreRenderInfo semaphoreInfo : renderedInfo.semaphoreInfo) {
				waitSemaphores.push_back(semaphoreInfo.semaphore);
				waitStages.push_back(semaphoreInfo.waitFlags);
				if (!semaphoreInfo.semaphore->IsBinary()) {
					waitValues.push_back(semaphoreInfo.waitValue);
				}
			}
			if (!sceneRenderContext->renderOnSwapchain) {
				texture->MakeImageReadableInShader(commandBuffer, PIPELINE_STAGE_FRAGMENT_SHADER, SHADER_READ);
			}
		}

		renderCommands.push_back(commandBuffer);
		SubmitInfo submitInfo;

		SemaphoreSubmitInfo waitSubmitInfo;
		waitSubmitInfo.semaphoreCount = waitSemaphores.size();
		waitSubmitInfo.semaphores = waitSemaphores.data();
		waitSubmitInfo.stageFlags = waitStages.data();
		waitSubmitInfo.timelineValues = waitValues.data();

		PipelineStageFlags signalStage = PIPELINE_STAGE_ALL_COMMANDS;

		SemaphoreSubmitInfo signalSubmitInfo;
		signalSubmitInfo.semaphoreCount = 1;
		signalSubmitInfo.stageFlags = &signalStage;
		signalSubmitInfo.semaphores = &imageRendererSemaphore;
		signalSubmitInfo.timelineValues = nullptr;

		submitInfo.waitSemaphoreInfo = waitSubmitInfo;
		submitInfo.signalSemaphoreInfo = signalSubmitInfo;

		
		
		Attachment colorAttachment;
		colorAttachment.texture = swapchainTexture;
		colorAttachment.clearBuffer = false;
		colorAttachment.texture = swapchainTexture;

		BeginRenderingInfo renderingInfo;
		renderingInfo.colorAttachmentCount = 1;
		renderingInfo.colorAttachments = &colorAttachment;
		renderingInfo.height = swapchain->GetHeight();
		renderingInfo.width = swapchain->GetWidth();

		renderingInfo.depthAttachment = nullptr;

		additionalSwapchainRenderer->Begin(commandBuffer, renderingInfo);
		OnCommandBufferRecorded.Broadcast(commandBuffer);
		additionalSwapchainRenderer->End(commandBuffer);
		

		swapchain->PreparePresentImage(commandBuffer);
		commandBuffer->End();

		graphicQueue->Submit(renderCommands.data(), renderCommands.size(), submitInfo, currentFence);

		PresentSwapchain();
		swapchain->AdvanceToNextImage();
	}

	void RendererController::PresentSwapchain() {
		EngineGraphicResources* engineResources = ServiceLocator::Get<EngineGraphicResources>();
		Device* device = engineResources->Device;
		Swapchain* swapchain = engineResources->Swapchain;

		uint32_t imageIndex = swapchain->GetImageIndex();
		Semaphore* imageRendererSemaphore = engineResources->GetImageRenderedSemaphore(imageIndex);

		PresentInfo presentInfo;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.waitSemaphores = &imageRendererSemaphore;
		presentInfo.swapchain = swapchain;

		if (!presentQueue->Present(presentInfo)) {
			device->WaitIdle();
			swapchain->Recreate(device);
		}
	}

	void RendererController::Destroy() {
		EngineGraphicResources* engineResources = ServiceLocator::Get<EngineGraphicResources>();
		Device* device = engineResources->Device;
		uint32_t frameCount = engineResources->GetFrameInFlightCount();
		
		shouldStop = true;
		WaitRender();
		OnBeginDestroy.Broadcast();

		for (uint32_t i = 0; i < frameCount; i++) {
			renderInterface->DestroyCommandBuffer(graphicCommandBuffers[i]);
		}
		for (std::unique_ptr<SceneRenderContext>& sceneRenderContext : renderContexts) {
			sceneRenderContext->sceneRenderer->Destroy(device);
		}
		renderInterface->DestroyDynamicRenderer(additionalSwapchainRenderer);
	}
}