#include "IVulkan/vulkan_device_queue.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/vulkan_fence.hpp"
#include "IVulkan/vulkan_semaphore.hpp"
#include "IVulkan/vulkan_command_buffer.hpp"
#include "IVulkan/enums/vulkan_synchronization_stage.hpp"
#include "Volk/volk.h"

namespace rhi {
	
	void VulkanDeviceQueue::Create(VkDevice vulkanDevice, uint32_t _familyIndex) {
		familyIndex = _familyIndex;
		vkGetDeviceQueue(vulkanDevice, familyIndex, 0, &queue);
		
	}
	bool VulkanDeviceQueue::PresentVk(VulkanPresentInfo presentInfo) {
		std::vector<VkSemaphore> waitSemaphores(presentInfo.waitSemaphoreCount);

		for (uint32_t i = 0; i < waitSemaphores.size(); i++) {
			VulkanSemaphore& vulkanSemaphore = presentInfo.waitSemaphores[i];
			if (!vulkanSemaphore.IsBinary()) {
				throw std::exception("Sempahore for present must be binary");
				return false;
			}
			waitSemaphores[i] = vulkanSemaphore.GetSemaphore();
		}

		uint32_t imageIndex = presentInfo.swapchain.GetImageIndex();
		VkSwapchainKHR swapchain = presentInfo.swapchain.GetSwapchain();

		VkPresentInfoKHR vkPresentInfo{};
		vkPresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		vkPresentInfo.pImageIndices = &imageIndex;
		vkPresentInfo.pSwapchains = &swapchain;
		vkPresentInfo.swapchainCount = 1;
		vkPresentInfo.pWaitSemaphores = waitSemaphores.data();
		vkPresentInfo.waitSemaphoreCount = waitSemaphores.size();

		VkResult result = vkQueuePresentKHR(queue, &vkPresentInfo);
		return (result == VK_SUCCESS);
		
	}
	void VulkanDeviceQueue::SubmitVk(VulkanCommandBuffer* commandBuffers, uint32_t commandBufferCount, VulkanFence* fence) {
		VkSubmitInfo2 queueSubmitInfo{};
		queueSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
		VkFence vkFence = fence ? fence->GetFence() : VK_NULL_HANDLE;

		std::vector<VkCommandBufferSubmitInfo> commandBufferInfo(commandBufferCount);

		for (uint32_t i = 0; i < commandBufferCount; i++) {
			commandBufferInfo[i].sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
			commandBufferInfo[i].commandBuffer = commandBuffers[i].commandBuffer;
		}

		queueSubmitInfo.pCommandBufferInfos = commandBufferInfo.data();
		queueSubmitInfo.commandBufferInfoCount = commandBufferInfo.size();

		queueSubmitInfo.signalSemaphoreInfoCount = 0;
		queueSubmitInfo.waitSemaphoreInfoCount = 0;

		vkQueueSubmit2(queue, 1, &queueSubmitInfo, vkFence);
	}
	void VulkanDeviceQueue::SubmitVk(VulkanCommandBuffer* commandBuffers, uint32_t commandBufferCount, VulkanSubmitInfo submitInfo, VulkanFence* fence) {
		VkSubmitInfo2 queueSubmitInfo{};
		queueSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
		VkFence vkFence = fence ? fence->GetFence() : VK_NULL_HANDLE;

		std::vector<VkCommandBufferSubmitInfo> commandBufferInfo (commandBufferCount);
		
		for (uint32_t i = 0; i < commandBufferCount; i++) {
			commandBufferInfo[i].sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
			commandBufferInfo[i].commandBuffer = commandBuffers[i].commandBuffer;
		}
		SemaphoreSubmitInfoVk waitSempahoreInfo = submitInfo.waitSemaphoreInfo;
		SemaphoreSubmitInfoVk signalSemaphoreInfo = submitInfo.signalSemaphoreInfo;

		std::vector<VkSemaphoreSubmitInfo> waitInfo = GetSubmitInfo(submitInfo.waitSemaphoreInfo);
		std::vector<VkSemaphoreSubmitInfo> signalInfo = GetSubmitInfo(submitInfo.signalSemaphoreInfo);
		
		queueSubmitInfo.pCommandBufferInfos = commandBufferInfo.data();
		queueSubmitInfo.commandBufferInfoCount = commandBufferInfo.size();

		queueSubmitInfo.pWaitSemaphoreInfos = waitInfo.data();
		queueSubmitInfo.waitSemaphoreInfoCount = waitInfo.size();
		
		queueSubmitInfo.pSignalSemaphoreInfos = signalInfo.data();
		queueSubmitInfo.signalSemaphoreInfoCount = signalInfo.size();

		VkResult result = vkQueueSubmit2(queue, 1, &queueSubmitInfo, vkFence);
		
	}

	std::vector<VkSemaphoreSubmitInfo> VulkanDeviceQueue::GetSubmitInfo(SemaphoreSubmitInfoVk submitInfo) {
		std::vector<VkSemaphoreSubmitInfo> semaphoreSubmitInfo (submitInfo.semaphoreCount);
		
		uint32_t binaryIndex = semaphoreSubmitInfo.size() - 1;
		uint32_t timelineIndex = 0;

		for (uint32_t i = 0; i < semaphoreSubmitInfo.size(); i++) {
			VulkanSemaphore vulkanSemaphore = submitInfo.semaphores[i];
			semaphoreSubmitInfo[i].sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;

			if (vulkanSemaphore.IsBinary()) {
				semaphoreSubmitInfo[binaryIndex].semaphore = vulkanSemaphore.GetSemaphore();
				semaphoreSubmitInfo[binaryIndex].value = 0;
				semaphoreSubmitInfo[binaryIndex].stageMask = submitInfo.stageFlags[i];
				binaryIndex--;
			}
			else {
				semaphoreSubmitInfo[timelineIndex].semaphore = vulkanSemaphore.GetSemaphore();
				semaphoreSubmitInfo[timelineIndex].value = submitInfo.timelineValues[timelineIndex];
				semaphoreSubmitInfo[timelineIndex].stageMask = submitInfo.stageFlags[i];
				timelineIndex++;
			}
		}
		return semaphoreSubmitInfo;
	}
	

	void VulkanDeviceQueue::Submit(CommandBuffer** commandsBuffer, uint32_t commandBufferCount, SubmitInfo submitInfo, Fence* fence) {
		VulkanSubmitInfo vulkanSubmitInfo;
		SemaphoreSubmitInfo waitSemaphoreInfo = submitInfo.waitSemaphoreInfo;
		SemaphoreSubmitInfo signalSemaphoreInfo = submitInfo.signalSemaphoreInfo;

		std::vector<VulkanCommandBuffer> vulkanCommandBuffers(commandBufferCount);
		for (uint32_t i = 0; i < commandBufferCount; i++) {
			vulkanCommandBuffers[i] = commandsBuffer[i]->API_VULKAN();
		}

		std::vector<VulkanSemaphore> waitSemaphores(waitSemaphoreInfo.semaphoreCount);
		std::vector<VkPipelineStageFlags2> waitFlags(waitSemaphoreInfo.semaphoreCount);

		std::vector<VulkanSemaphore> signalSemaphores(signalSemaphoreInfo.semaphoreCount);
		std::vector<VkPipelineStageFlags2> signalFlags(signalSemaphoreInfo.semaphoreCount);

		for (uint32_t i = 0; i < waitSemaphores.size(); i++) {
			waitSemaphores[i] = waitSemaphoreInfo.semaphores[i]->API_VULKAN();
			waitFlags[i] = ToVulkanPipelineStage2(waitSemaphoreInfo.stageFlags[i]);
		}

		for (uint32_t i = 0; i < signalSemaphores.size(); i++) {
			signalSemaphores[i] = signalSemaphoreInfo.semaphores[i]->API_VULKAN();
			signalFlags[i] = ToVulkanPipelineStage2(signalSemaphoreInfo.stageFlags[i]);
		}

		vulkanSubmitInfo.waitSemaphoreInfo.semaphores = waitSemaphores.data();
		vulkanSubmitInfo.waitSemaphoreInfo.semaphoreCount = waitSemaphoreInfo.semaphoreCount;
		vulkanSubmitInfo.waitSemaphoreInfo.stageFlags = waitFlags.data();
		vulkanSubmitInfo.waitSemaphoreInfo.timelineValues = waitSemaphoreInfo.timelineValues;

		vulkanSubmitInfo.signalSemaphoreInfo.semaphores = signalSemaphores.data();
		vulkanSubmitInfo.signalSemaphoreInfo.semaphoreCount = signalSemaphoreInfo.semaphoreCount;
		vulkanSubmitInfo.signalSemaphoreInfo.stageFlags = signalFlags.data();
		vulkanSubmitInfo.signalSemaphoreInfo.timelineValues = signalSemaphoreInfo.timelineValues;

		
		if (fence) {
			SubmitVk(vulkanCommandBuffers.data(), commandBufferCount, vulkanSubmitInfo, &fence->API_VULKAN());
		}
		else {
			SubmitVk(vulkanCommandBuffers.data(), commandBufferCount, vulkanSubmitInfo, nullptr);
		}
	}

	void VulkanDeviceQueue::Submit(CommandBuffer** commandsBuffer, uint32_t commandBufferCount, Fence* fence) {
		std::vector<VulkanCommandBuffer> vulkanCommandBuffers(commandBufferCount);
		for (uint32_t i = 0; i < commandBufferCount; i++) {
			vulkanCommandBuffers[i] = commandsBuffer[i]->API_VULKAN();
		}
		if (fence) {
			SubmitVk(vulkanCommandBuffers.data(), commandBufferCount, &fence->API_VULKAN());
		}
		else {
			SubmitVk(vulkanCommandBuffers.data(), commandBufferCount, nullptr);
		}
	}

	bool VulkanDeviceQueue::Present(PresentInfo presentInfo) {
		VulkanSwapchain& vulkanSwapchain = presentInfo.swapchain->API_VULKAN();

		VulkanPresentInfo vulkanPresentInfo;
		std::vector<VulkanSemaphore> waitSemaphores(presentInfo.waitSemaphoreCount);
		
		for (uint32_t i = 0; i < waitSemaphores.size(); i++) {
			waitSemaphores[i] = presentInfo.waitSemaphores[i]->API_VULKAN();
		}

		vulkanPresentInfo.waitSemaphores = waitSemaphores.data();
		vulkanPresentInfo.waitSemaphoreCount = waitSemaphores.size();
		vulkanPresentInfo.swapchain = vulkanSwapchain;

		return PresentVk(vulkanPresentInfo);
	}
}