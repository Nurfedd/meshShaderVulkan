#include "IVulkan/vulkan_barriers.hpp"
#include "IVulkan/enums/vulkan_synchronization_stage.hpp"
#include "IVulkan/vulkan_buffer.hpp"
#include "IVulkan/vulkan_image.hpp"
#include "IVulkan/vulkan_texture.hpp"
namespace rhi {
	// VK IMPL
	void VulkanBarrier::SetStages(VkPipelineStageFlags2 srcStage, VkAccessFlags2 srcAccess, VkPipelineStageFlags2 dstStage, VkAccessFlags2 dstAccess) {
		srcStageMask = srcStage;
		srcAccessMask = srcAccess;
		dstStageMask = dstStage;
		dstAccessMask = dstAccess;
	}

	VkMemoryBarrier2 VulkanMemoryBarrier::GetBarrier() {
		VkMemoryBarrier2 memoryBarrier{};
		memoryBarrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER_2;

		memoryBarrier.srcAccessMask = srcAccessMask;
		memoryBarrier.srcStageMask = srcStageMask;
		memoryBarrier.dstAccessMask = dstAccessMask;
		memoryBarrier.dstStageMask = dstStageMask;

		return memoryBarrier;
	}

	VkBufferMemoryBarrier2 VulkanBufferBarrier::GetBarrier() {
		VkBufferMemoryBarrier2 bufferBarrier{};
		bufferBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2;

		bufferBarrier.srcAccessMask = srcAccessMask;
		bufferBarrier.srcStageMask = srcStageMask;
		bufferBarrier.dstAccessMask = dstAccessMask;
		bufferBarrier.dstStageMask = dstStageMask;

		bufferBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		bufferBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		bufferBarrier.buffer = buffer;
		bufferBarrier.size = size;
		bufferBarrier.offset = offset;
		return bufferBarrier;
	}
	VkImageMemoryBarrier2 VulkanImageBarrier::GetBarrier() {
		VkImageMemoryBarrier2 imageBarrier{};
		imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;

		imageBarrier.srcAccessMask = srcAccessMask;
		imageBarrier.srcStageMask = srcStageMask;
		imageBarrier.dstAccessMask = dstAccessMask;
		imageBarrier.dstStageMask = dstStageMask;

		imageBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		imageBarrier.image = image;
		imageBarrier.newLayout = newLayout;
		imageBarrier.oldLayout = oldLayout;

		imageBarrier.subresourceRange = subresourceRange;              
		
		return imageBarrier;
	}
	// RHI IMPL
	VulkanBarrier::VulkanBarrier(const MemoryBarrier& barrier) {
		srcStageMask = ToVulkanPipelineStage2(barrier.srcStageMask);
		srcAccessMask = ToVulkanAccessFlags2(barrier.srcAccessMask);
		dstStageMask = ToVulkanPipelineStage2(barrier.dstStageMask);
		dstAccessMask = ToVulkanAccessFlags2(barrier.dstAccessMask);
	}

	VulkanMemoryBarrier::VulkanMemoryBarrier(MemoryBarrier barrier) : VulkanBarrier(barrier) {}

	VulkanBufferBarrier::VulkanBufferBarrier(const BufferBarrier& bufferBarrier) : VulkanBarrier(bufferBarrier) {
		buffer = bufferBarrier.buffer->API_VULKAN().GetBuffer();
	}

	VulkanImageBarrier::VulkanImageBarrier(const ImageBarrier& imageBarrier) : VulkanBarrier(imageBarrier) {
		image = imageBarrier.image->API_VULKAN().GetImage()->GetVkImage();
		oldLayout = ToVulkanImageLayout(imageBarrier.oldLayout);
		newLayout = ToVulkanImageLayout(imageBarrier.newLayout);
	}
}