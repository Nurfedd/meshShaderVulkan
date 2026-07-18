#include "IVulkan/vulkan_texture.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/enums/vulkan_format.hpp"
#include "IVulkan/enums/vulkan_image_type.hpp"
#include "IVulkan/vulkan_command_buffer.hpp"
#include "IVulkan/vulkan_buffer.hpp"
#include "IVulkan/enums/vulkan_synchronization_stage.hpp"
#include <algorithm>

namespace rhi {
	VulkanTextureCreateInfo::VulkanTextureCreateInfo(TextureCreateInfo createInfo) {
		textureFormat = ToVulkanFormat(createInfo.textureFormat);
		imageUsage = ToVulkanImageUsage(createInfo.imageUsage);
		viewType = ToVulkanViewType(createInfo.viewType);
		mipCount = createInfo.mipCount;
		width = createInfo.width;
		height = createInfo.height;
	}
	void VulkanTexture::CreateVk(VulkanDevice& vulkanDevice, VulkanTextureCreateInfo createInfo) {
		
		textureWidth = createInfo.width;
		textureHeight = createInfo.height;
		vkImageUsage = createInfo.imageUsage | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT; // upload and image copy

		VulkanImageCreateInfo imageCreateInfo;
		imageCreateInfo.arrayLayers = GetLayerCountByViewType(FromVulkanViewType(createInfo.viewType));
		imageCreateInfo.mipLevel = createInfo.mipCount;
		imageCreateInfo.usageFlags = vkImageUsage;
		imageCreateInfo.format = createInfo.textureFormat;
		imageCreateInfo.height = createInfo.height;
		imageCreateInfo.width = createInfo.width;

		VulkanImage newImage;
		newImage.Create(vulkanDevice, imageCreateInfo);

		VulkanImageView newImageView;
		newImageView.Create(vulkanDevice, newImage, createInfo.viewType);
		vulkanImage = newImage;
		vulkanImageView = newImageView;
	}
	
	void VulkanTexture::UploadVk(VulkanDevice& vulkanDevice, VulkanUploadContext& uploadContext, void* pixels, uint32_t channels) {
		uint32_t imageSize = textureHeight * textureWidth * channels;

		VulkanBuffer staging;
		staging.CreateCpuVk(vulkanDevice, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, pixels, imageSize);
		
		VkExtent3D extent;
		extent.width = textureWidth;
		extent.height = textureHeight;
		extent.depth = 1;

		int width = textureWidth;
		int height = textureHeight;

		VkImage image = vulkanImage.GetVkImage();

		uint32_t mipLevel = vulkanImage.GetMipLevel();

		uploadContext.Upload(vulkanDevice,[&](VulkanCommandBuffer cmd) {
			

			VkBufferImageCopy region{};
			region.bufferOffset = 0;
			region.bufferRowLength = 0;
			region.bufferImageHeight = 0;

			region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			region.imageSubresource.mipLevel = 0;
			region.imageSubresource.baseArrayLayer = 0;
			region.imageSubresource.layerCount = 1;

			region.imageOffset = { 0, 0, 0 };
			region.imageExtent = extent;

			vulkanImage.TransitionImage(
				cmd,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				VK_PIPELINE_STAGE_2_TRANSFER_BIT,
				VK_ACCESS_2_TRANSFER_WRITE_BIT,
				0, 1,
				VK_IMAGE_ASPECT_COLOR_BIT
			);
			vkCmdCopyBufferToImage(
				cmd.commandBuffer,
				staging.GetBuffer(),
				image,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1,
				&region
			);

			vulkanImage.TransitionImage(
				cmd,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				VK_PIPELINE_STAGE_2_TRANSFER_BIT,
				VK_PIPELINE_STAGE_2_TRANSFER_BIT,
				VK_ACCESS_2_TRANSFER_WRITE_BIT,
				VK_ACCESS_2_TRANSFER_READ_BIT,
				0, 1,
				VK_IMAGE_ASPECT_COLOR_BIT
			);

			UploadMipmap(cmd);
			});
		staging.Destroy(vulkanDevice);
	}

	void VulkanTexture::UploadMipmap(VulkanCommandBuffer cmd)
	{
		int width = textureWidth;
		int height = textureHeight;

		VkImage image = vulkanImage.GetVkImage();
		uint32_t mipLevel = vulkanImage.GetMipLevel();
		

		for (uint32_t i = 1; i < mipLevel; i++)
		{
			int mipWidth = std::max(1, width >> i);
			int mipHeight = std::max(1, height >> i);

			int prevWidth = std::max(1, width >> (i - 1));
			int prevHeight = std::max(1, height >> (i - 1));

			
			vulkanImage.TransitionImage(
				cmd,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				VK_PIPELINE_STAGE_2_TRANSFER_BIT,
				VK_PIPELINE_STAGE_2_TRANSFER_BIT,
				0,
				VK_ACCESS_2_TRANSFER_WRITE_BIT,
				i, 1,
				VK_IMAGE_ASPECT_COLOR_BIT
			);

			
			VkImageBlit blit{};
			blit.srcSubresource = {
				VK_IMAGE_ASPECT_COLOR_BIT,
				i - 1,
				0,
				1
			};
			blit.srcOffsets[0] = { 0, 0, 0 };
			blit.srcOffsets[1] = { prevWidth, prevHeight, 1 };

			blit.dstSubresource = {
				VK_IMAGE_ASPECT_COLOR_BIT,
				i,
				0,
				1
			};
			blit.dstOffsets[0] = { 0, 0, 0 };
			blit.dstOffsets[1] = { mipWidth, mipHeight, 1 };

			vkCmdBlitImage(
				cmd.commandBuffer,
				image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1, &blit,
				VK_FILTER_LINEAR
			);

			
			vulkanImage.TransitionImage(
				cmd,
				VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
				VK_PIPELINE_STAGE_2_TRANSFER_BIT,
				VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT,
				VK_ACCESS_2_TRANSFER_READ_BIT,
				VK_ACCESS_2_SHADER_READ_BIT,
				i - 1, 1,
				VK_IMAGE_ASPECT_COLOR_BIT
			);

			
			vulkanImage.TransitionImage(
				cmd,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				VK_PIPELINE_STAGE_2_TRANSFER_BIT,
				VK_PIPELINE_STAGE_2_TRANSFER_BIT,
				VK_ACCESS_2_TRANSFER_WRITE_BIT,
				VK_ACCESS_2_TRANSFER_READ_BIT,
				i, 1,
				VK_IMAGE_ASPECT_COLOR_BIT
			);
		}

		
		vulkanImage.TransitionImage(
			cmd,
			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			VK_PIPELINE_STAGE_2_TRANSFER_BIT,
			VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT,
			VK_ACCESS_2_TRANSFER_READ_BIT,
			VK_ACCESS_2_SHADER_READ_BIT,
			mipLevel - 1, 1,
			VK_IMAGE_ASPECT_COLOR_BIT
		);
	}
	void VulkanTexture::Upload(Device* device, UploadContext* uploadContext, void* pixels, uint32_t channels) {
		UploadVk(device->API_VULKAN(), uploadContext->API_VULKAN(), pixels, channels);
	}

	bool VulkanTexture::MakeImageReadableInShader(CommandBuffer* commandBuffer, PipelineStageFlags dstStage, AccessFlags accessFlags) {
		VulkanCommandBuffer& vulkanCommandBuffer = commandBuffer->API_VULKAN();
		VkImageAspectFlags aspectFlags = (vulkanImage.GetFormat() == VK_FORMAT_D32_SFLOAT) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
		if (vkImageUsage & VK_IMAGE_USAGE_SAMPLED_BIT || vkImageUsage & VK_IMAGE_USAGE_STORAGE_BIT) {
			vulkanImage.TransitionImage(
				vulkanCommandBuffer,
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
				ToVulkanPipelineStage2(dstStage),
				ToVulkanAccessFlags2(accessFlags),
				aspectFlags
			);
			return true;
		}
		return false;
	}
	bool VulkanTexture::CanBeUsedInShader() {
		return (vulkanImage.GetCurrentLayout() == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

	void VulkanTexture::ResizeVk(VulkanDevice& vulkanDevice, uint32_t width, uint32_t height) {
		VulkanTextureCreateInfo textureCreateInfo;
		textureCreateInfo.height = height;
		textureCreateInfo.width = width;
		textureCreateInfo.imageUsage = vkImageUsage;
		textureCreateInfo.textureFormat = vulkanImage.GetFormat();
		textureCreateInfo.mipCount = vulkanImage.GetMipLevel();
		textureCreateInfo.viewType = vulkanImageView.GetViewType();

		DestroyVk(vulkanDevice);
		CreateVk(vulkanDevice, textureCreateInfo);
		
	}
	void VulkanTexture::Resize(Device* device, uint32_t width, uint32_t height) {
		ResizeVk(device->API_VULKAN(), width, height);
	}
	
	void VulkanTexture::Create(Device* device, TextureCreateInfo createInfo) {
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		CreateVk(vulkanDevice, createInfo);
	}

	void VulkanTexture::DestroyVk(VulkanDevice& vulkanDevice) {
		vulkanImageView.Destroy(vulkanDevice);
		vulkanImage.Destroy(vulkanDevice);
	}

	void VulkanTexture::Destroy(Device* device) {
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		DestroyVk(vulkanDevice);
	}
}