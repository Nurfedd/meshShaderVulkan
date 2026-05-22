#include "IVulkan/vulkan_image.hpp"
#include "IVulkan/enums/vulkan_format.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/vulkan_command_buffer.hpp"
#include "IVulkan/enums/vulkan_image_type.hpp"
#include "Volk/volk.h"
namespace rhi {
	void VulkanImage::CreateFromVkImage(VkImage vkImage,VkFormat vkFormat) {
		image = vkImage;
        imageFormat = vkFormat;
        mipLevel = 1;
        arrayLayer = 1;  
	}
    void VulkanImage::Create(VulkanDevice& vulkanDevice, VulkanImageCreateInfo createInfo) {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent = { createInfo.width, createInfo.height, 1 };
        imageInfo.mipLevels = createInfo.mipLevel;
        imageInfo.arrayLayers = createInfo.arrayLayers;
        imageInfo.format = createInfo.format;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = createInfo.usageFlags;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo allocInfo{};
        allocInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
        mipLevel = createInfo.mipLevel;
        arrayLayer = createInfo.arrayLayers;
        imageFormat = createInfo.format;
        usageFlags = createInfo.usageFlags;

        vmaCreateImage(vulkanDevice.GetAllocator(), &imageInfo, &allocInfo, &image, &allocation, nullptr);
    }

   
	void VulkanImage::Destroy(VulkanDevice& vulkanDevice) {
        if (allocation) {
            vmaDestroyImage(vulkanDevice.GetAllocator(), image, allocation);
        }
        else {
            vkDestroyImage(vulkanDevice, image, nullptr);
        }
	}

    void VulkanImage::TransitionImage(VulkanCommandBuffer cmdBuffer, VkImageLayout newLayout, VkPipelineStageFlags2 dstStage, VkAccessFlags2 dstAccess, VkImageSubresourceRange subresrouceRange) {
       
        VulkanImageBarrier barrier;
        barrier.image = image;
        barrier.oldLayout = currentLayout;
        barrier.newLayout = newLayout;
        barrier.srcStageMask = currentStage;
        barrier.srcAccessMask = currentAccess;
        barrier.dstStageMask = dstStage;
        barrier.dstAccessMask = dstAccess;
        
        barrier.subresourceRange = subresrouceRange;

        BarriersDependencyVk dep{};
        dep.imageBarrierCount = 1;
        dep.imageBarriers = &barrier;

        cmdBuffer.PipelineBarrierVk(dep);

        currentLayout = newLayout;
        currentAccess = dstAccess;
        currentStage = dstStage;
    }

    void VulkanImage::TransitionImage(VulkanCommandBuffer cmdBuffer, VkImageLayout newLayout, VkPipelineStageFlags2 dstStage, VkAccessFlags2 dstAccess, VkImageAspectFlags aspectMask) {
        VkImageSubresourceRange subresourceRange{};
        subresourceRange.baseMipLevel = 0;
        subresourceRange.levelCount = mipLevel;
        subresourceRange.baseArrayLayer = 0;
        subresourceRange.layerCount = arrayLayer;
        subresourceRange.aspectMask = aspectMask;
        TransitionImage(cmdBuffer, newLayout, dstStage, dstAccess, subresourceRange);
    }
    void VulkanImage::TransitionImage(VulkanCommandBuffer cmdBuffer, VkImageLayout newLayout, VkPipelineStageFlags2 dstStage, VkAccessFlags2 dstAccess, uint32_t baseMip, uint32_t mipCount) {

        VkImageSubresourceRange subresourceRange{};
        subresourceRange.baseMipLevel = baseMip;
        subresourceRange.levelCount = mipCount;
        subresourceRange.layerCount = arrayLayer;
        subresourceRange.baseArrayLayer = 0;
        TransitionImage(cmdBuffer, newLayout, dstStage, dstAccess, subresourceRange);
    }

    void VulkanImage::TransitionImage(VulkanCommandBuffer cmdBuffer, VkImageLayout newLayout, VkPipelineStageFlags2 dstStage, VkAccessFlags2 dstAccess, uint32_t baseMip, uint32_t mipCount, VkImageAspectFlags aspectMask) {

        VkImageSubresourceRange subresourceRange{};
        subresourceRange.baseMipLevel = baseMip;
        subresourceRange.levelCount = mipCount;
        subresourceRange.layerCount = arrayLayer;
        subresourceRange.aspectMask = aspectMask;
        subresourceRange.baseArrayLayer = 0;
        TransitionImage(cmdBuffer, newLayout, dstStage, dstAccess, subresourceRange);
    }

    void VulkanImage::TransitionImage(VulkanCommandBuffer cmdBuffer, VkImageLayout oldLayout, VkImageLayout newLayout, VkPipelineStageFlags2 srcStage, VkPipelineStageFlags2 dstStage, VkAccessFlags2 srcAccess, VkAccessFlags2 dstAccess, uint32_t baseMip, uint32_t mipCount, VkImageAspectFlags aspectMask) {

        VkImageSubresourceRange subresourceRange{};
        subresourceRange.aspectMask = aspectMask;
        subresourceRange.baseMipLevel = baseMip;
        subresourceRange.levelCount = mipCount;
        subresourceRange.baseArrayLayer = 0;
        subresourceRange.layerCount = arrayLayer;

        VkImageMemoryBarrier2 barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcStageMask = srcStage;
        barrier.dstStageMask = dstStage;
        barrier.srcAccessMask = srcAccess;
        barrier.dstAccessMask = dstAccess;
        barrier.subresourceRange = subresourceRange;
        barrier.image = image;

        VkDependencyInfo dep{};
        dep.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
        dep.imageMemoryBarrierCount = 1;
        dep.pImageMemoryBarriers = &barrier;

        vkCmdPipelineBarrier2(cmdBuffer.commandBuffer, &dep);
    }
    

    void VulkanImage::TransitionImage(VulkanCommandBuffer cmdBuffer, VkImageLayout newLayout, VkPipelineStageFlags2 dstStage, VkAccessFlags2 dstAccess) {
        VkImageSubresourceRange subresourceRange{};
        subresourceRange.baseMipLevel = 0;
        subresourceRange.levelCount = mipLevel;
        subresourceRange.baseArrayLayer = 0;
        subresourceRange.layerCount = arrayLayer;
        TransitionImage(cmdBuffer, newLayout, dstStage, dstAccess,subresourceRange);
    }

	
}