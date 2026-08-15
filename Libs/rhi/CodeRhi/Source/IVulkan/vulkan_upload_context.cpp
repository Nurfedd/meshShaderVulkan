#include "IVulkan/vulkan_upload_context.hpp"
#include "IVulkan/vulkan_device.hpp"
namespace rhi {
	void VulkanUploadContext::Create(Device* device) {
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		DeviceQueue* graphicQueue = device->GetGraphicQueue();
		queue = &graphicQueue->API_VULKAN();

		fence.CreateVk(vulkanDevice, VK_FENCE_CREATE_SIGNALED_BIT);
		commandPool.Create(device, graphicQueue);
		commandPool.AllocateCommandBufferVk(vulkanDevice, &commandBuffer.commandBuffer, 1, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		
	}

	void VulkanUploadContext::Upload(VulkanDevice& vulkanDevice,std::function<void(VulkanCommandBuffer commandBuffer)> func) {
		
		vulkanDevice.WaitForFencesVk(&fence, 1, true);
		vulkanDevice.ResetFencesVk(&fence, 1);
		
		commandBuffer.Reset();
		commandBuffer.BeginVk(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		func(commandBuffer);
		commandBuffer.End();
		
		queue->SubmitVk(&commandBuffer, 1, &fence);
		vulkanDevice.WaitForFencesVk(&fence, 1, true);
	}
	void VulkanUploadContext::Destroy(Device* device) {
		commandPool.Destroy(device);
		fence.Destroy(device);
	}

	void VulkanUploadContext::DestroyVk(VulkanDevice& vulkanDevice) {
		fence.DestroyVk(vulkanDevice);
		commandPool.DestroyVk(vulkanDevice);
	}
} 