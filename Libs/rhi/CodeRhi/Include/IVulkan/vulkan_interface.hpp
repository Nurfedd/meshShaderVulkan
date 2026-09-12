#pragma once
#include "Interface/render_interface.hpp"
#include "IVulkan/vulkan_instance.hpp"
#include "IVulkan/vulkan_surface.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/vulkan_swapchain.hpp"
#include "IVulkan/vulkan_shader.hpp"
#include "IVulkan/vulkan_set_layout.hpp"
#include "IVulkan/vulkan_command_pool.hpp"
#include "IVulkan/vulkan_command_buffer.hpp"
#include "IVulkan/vulkan_graphic_pipeline.hpp"
#include "IVulkan/vulkan_compute_pipeline.hpp"
#include "IVulkan/vulkan_descriptor_pool.hpp"
#include "IVulkan/vulkan_descriptor_set.hpp"
#include "IVulkan/vulkan_semaphore.hpp"
#include "IVulkan/vulkan_fence.hpp"
#include "IVulkan/vulkan_upload_context.hpp"
#include "IVulkan/vulkan_buffer.hpp"
#include "IVulkan/vulkan_sampler.hpp"
#include "IVulkan/vulkan_dynamic_renderer.hpp"
#include "IVulkan/vulkan_texture.hpp"

namespace rhi {
	class VulkanInterface : public RenderInterface {
	public:
		VulkanInstance* InitInstance() override { return new VulkanInstance; }
		VulkanSurface* InitSurface() override { return new VulkanSurface; }
		VulkanDevice* InitDevice() override { return new VulkanDevice; }
		VulkanSwapchain* InitSwapchain() override { return new VulkanSwapchain; }
		VulkanShader* InitShader() override { return new VulkanShader; }
		VulkanSetLayout* InitSetLayout() override { return new VulkanSetLayout; }
		VulkanCommandPool* InitCommandPool() override { return new VulkanCommandPool; }
		VulkanCommandBuffer* InitCommandBuffer() override { return new VulkanCommandBuffer; }
		VulkanGraphicPipeline* InitGraphicPipeline() override { return new VulkanGraphicPipeline; }
		VulkanComputePipeline* InitComputePipeline() override { return new VulkanComputePipeline; }
		VulkanDescriptorPool* InitDescriptorPool() override { return new VulkanDescriptorPool; }
		VulkanDescriptorSet* InitDescriptorSet() override { return new VulkanDescriptorSet; }
		VulkanSemaphore* InitSemaphore() override { return new VulkanSemaphore; }
		VulkanFence* InitFence() override { return new VulkanFence; }
		VulkanUploadContext* InitUploadContext() override { return new VulkanUploadContext; }
		VulkanBuffer* InitBuffer() override { return new VulkanBuffer; }
		VulkanSampler* InitSampler() override { return new VulkanSampler; }
		VulkanDynamicRenderer* InitDynamicRenderer() override { return new VulkanDynamicRenderer; }
		VulkanTexture* InitTexture() override { return new VulkanTexture; }
	};
}