#pragma once
#include "Interface/command_buffer.hpp"
#include "Interface/command_pool.hpp"
#include "Interface/device.hpp"
#include "Interface/texture.hpp"
#include "Interface/semaphore.hpp"
namespace nino_engine {
	struct SemaphoreRenderInfo {
		SemaphoreRenderInfo(rhi::Semaphore* _semaphore, rhi::PipelineStageFlags _waitFlags) {
			semaphore = _semaphore;
			waitFlags = _waitFlags;
		}
		rhi::Semaphore* semaphore = nullptr;
		rhi::PipelineStageFlags waitFlags;
		uint64_t waitValue; // used for timeline semaphore
	};
	struct SceneRenderedInfo {
		std::vector<SemaphoreRenderInfo> semaphoreInfo;
		rhi::CommandBuffer* commandToSubmit = nullptr;
		uint32_t semaphoreCount = 0;
	};
	class SceneRenderer {
	public :
		virtual ~SceneRenderer() = default;
		virtual void Create(rhi::Device* device);
		
		virtual SceneRenderedInfo RenderFrame(rhi::Texture* texture) = 0;
		virtual void Destroy(rhi::Device* device);
	protected :
		//virtual void RecordCommand();

		rhi::DeviceQueue* graphicQueue = nullptr;
		rhi::DeviceQueue* presentQueue = nullptr;
		rhi::DeviceQueue* computeQueue = nullptr;

		std::vector<rhi::CommandBuffer*> graphicCommandBuffers;

		rhi::CommandBuffer* GetCurrentCommandBuffer();
	};
}