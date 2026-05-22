#pragma once
#include "vulkan/vulkan.h"
#include "Interface/barriers.hpp"
namespace rhi {
	class VulkanBarrier {
	public :
		VulkanBarrier() = default;
		VulkanBarrier(const MemoryBarrier& barrier);
		VkPipelineStageFlags2 srcStageMask;
		VkAccessFlags2 srcAccessMask;
		VkPipelineStageFlags2 dstStageMask;
		VkAccessFlags2 dstAccessMask;

		void SetStages(VkPipelineStageFlags2 srcStage, VkAccessFlags2 srcAccess, VkPipelineStageFlags2 dstStage, VkAccessFlags2 dstAccess);
	};
	class VulkanMemoryBarrier : public VulkanBarrier {
	public :
		VulkanMemoryBarrier() = default;
		VulkanMemoryBarrier(MemoryBarrier barrier);
		VkMemoryBarrier2 GetBarrier();
	};
	class VulkanBufferBarrier : public VulkanBarrier{
	public :
		VulkanBufferBarrier() = default;
		VulkanBufferBarrier(const BufferBarrier& bufferBarrier);
		VkDeviceSize offset = 0;
		VkDeviceSize size = VK_WHOLE_SIZE;
		VkBuffer buffer;
		VkBufferMemoryBarrier2 GetBarrier();
	};

	class VulkanImageBarrier : public VulkanBarrier {
	public :
		VulkanImageBarrier() = default;
		VulkanImageBarrier(const ImageBarrier& imageBarrier);
		VkImageLayout oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		VkImageLayout newLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		VkImage image = VK_NULL_HANDLE;
		VkImageSubresourceRange subresourceRange;
		VkImageMemoryBarrier2 GetBarrier();
	};
}