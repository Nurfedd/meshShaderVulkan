#pragma once
#include "Interface/instance.hpp"
#include "Interface/surface.hpp"
#include "Interface/device.hpp"
#include "Interface/swapchain.hpp"
#include "Interface/shader.hpp"
#include "Interface/vertex_layout.hpp"
#include "Interface/set_layout.hpp"
#include "Interface/push_constant_range.hpp"
#include "Interface/command_pool.hpp"
#include "Interface/command_buffer.hpp"
#include "Interface/graphic_pipeline.hpp"
#include "Interface/compute_pipeline.hpp"
#include "Interface/descriptor_pool.hpp"
#include "Interface/descriptor_set.hpp"
#include "Interface/semaphore.hpp"
#include "Interface/fence.hpp"
#include "Interface/upload_context.hpp"
#include "Interface/sampler.hpp"
#include "Interface/dynamic_renderer.hpp"
#include "Interface/texture.hpp"


namespace rhi {
	class RenderInterface {
	public:
		// Create
		virtual Instance* InitInstance() = 0;
		virtual Surface* InitSurface() = 0;
		virtual Device* InitDevice() = 0;
		virtual Swapchain* InitSwapchain() = 0;
		virtual Shader* InitShader() = 0;
		virtual SetLayout* InitSetLayout() = 0;
		virtual CommandPool* InitCommandPool() = 0;
		virtual CommandBuffer* InitCommandBuffer() = 0;
		virtual GraphicPipeline* InitGraphicPipeline() = 0;
		virtual ComputePipeline* InitComputePipeline() = 0;
		virtual DescriptorPool* InitDescriptorPool() = 0;
		virtual DescriptorSet* InitDescriptorSet() = 0;
		virtual Semaphore* InitSemaphore() = 0;
		virtual Fence* InitFence() = 0;
		virtual UploadContext* InitUploadContext() = 0;
		virtual Buffer* InitBuffer() = 0;
		virtual Sampler* InitSampler() = 0;
		virtual DynamicRenderer* InitDynamicRenderer() = 0;
		virtual Texture* InitTexture() = 0;
		// Destroy
		void DestroyInstance(Instance* instance) { instance->Destroy(); delete instance; }
		void DestroySurface(Surface* surface, Instance* instance) { surface->Destroy(instance); delete surface; }
		void DestroyDevice(Device* device) { device->Destroy(); delete device; }
		void DestroySwapchain(Swapchain* swapchain, Device* device) { swapchain->Destroy(device); delete swapchain; }
		void DestroyShader(Shader* shader, Device* device) { shader->Destroy(device); delete shader; }
		void DestroySetLayout(SetLayout* setLayout, Device* device) { setLayout->Destroy(device); delete setLayout; }
		void DestroyCommandPool(CommandPool* commandPool, Device* device) { commandPool->Destroy(device); delete commandPool; }
		void DestroyCommandBuffer(CommandBuffer* commandBuffer) { delete commandBuffer; }
		void DestroyPipeline(Pipeline* pipeline, Device* device) { pipeline->Destroy(device); delete pipeline; }
		void DestroyDecriptorPool(DescriptorPool* descriptorPool, Device* device) { descriptorPool->Destroy(device); delete descriptorPool; }
		void DestroyDescriptorSet(DescriptorSet* descriptorSet) { delete descriptorSet; }
		void DestroySempahore(Semaphore* semaphore, Device* device) { semaphore->Destroy(device); delete semaphore; }
		void DestroyFence(Fence* fence, Device* device) { fence->Destroy(device); delete fence; }
		void DestroyUploadContext(UploadContext* uploadContext, Device* device) { uploadContext->Destroy(device); delete uploadContext; }
		void DestroyBuffer(Buffer* buffer, Device* device) { buffer->Destroy(device); delete buffer; }
		void DestroySampler(Sampler* sampler, Device* device) { sampler->Destroy(device); delete sampler; }
		void DestroyDynamicRenderer(DynamicRenderer* dynamicRenderer) { delete dynamicRenderer; }
		void DestroyTexture(Texture* texture, Device* device) { if (texture) { texture->Destroy(device); delete texture; } }
	};
}