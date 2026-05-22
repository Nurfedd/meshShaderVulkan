#pragma once
#include "Interface/upload_context.hpp"
#include "IVulkan/vulkan_command_pool.hpp"
#include "IVulkan/vulkan_command_buffer.hpp"
#include "IVulkan/vulkan_fence.hpp"
#include "IVulkan/vulkan_device_queue.hpp"
#include <functional>
namespace rhi {
	class VulkanUploadContext : public UploadContext {
	public :
		void Create(Device* device) override;
		void Upload(VulkanDevice& device,std::function<void(VulkanCommandBuffer commandBuffer)>);
		void Destroy(Device* device) override;
		void DestroyVk(VulkanDevice& vulkanDevice);
		VulkanUploadContext& API_VULKAN() override { return *(this); }
	private :
		VulkanCommandPool commandPool;
		VulkanCommandBuffer commandBuffer;
		VulkanDeviceQueue* queue;
		VulkanFence fence;
	};
}