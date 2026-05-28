#pragma once
#include "Interface/texture.hpp"
#include "IVulkan/vulkan_image.hpp"
#include "IVulkan/vulkan_image_view.hpp"
#include "IVulkan/vulkan_upload_context.hpp"
namespace rhi {
	struct VulkanTextureCreateInfo {
		VulkanTextureCreateInfo() = default;
		VulkanTextureCreateInfo(TextureCreateInfo createInfo);
		uint32_t width;
		uint32_t height;
		VkFormat textureFormat;
		VkImageUsageFlags imageUsage;
		uint32_t mipCount;
		VkImageViewType viewType;
	};
	class VulkanTexture : public Texture {
	public:
		void Create(Device* device, TextureCreateInfo createInfo) override;
		void CreateVk(VulkanDevice& vulkanDevice, VulkanTextureCreateInfo createInfo);
		void CreateVk(VulkanImage image, VulkanImageView imageView, uint32_t width, uint32_t height) { vulkanImage = image; vulkanImageView = imageView; textureWidth = width; textureHeight = height; };
		
		void UploadVk(VulkanDevice& vulkanDevice, VulkanUploadContext& uploadContext, void* pixels, uint32_t channels);
		void Upload(Device* device, UploadContext* uploadContext, void* pixels, uint32_t channels) override;
		bool MakeImageReadableInShader(CommandBuffer* commandBuffer,PipelineStageFlags dstStage, AccessFlags accessFlags) override;
		bool CanBeUsedInShader() override;
		void ResizeVk(VulkanDevice& vulkanDevice, uint32_t width, uint32_t height);
		void Resize(Device* device, uint32_t width, uint32_t height) override;
		void DestroyVk(VulkanDevice& vulkanDevice);
		void Destroy(Device* device) override;
		VulkanImage* GetImage() { return &vulkanImage; }
		VulkanImageView* GetImageView() { return &vulkanImageView; }
		VulkanTexture& API_VULKAN() override { return *(this); }

		uint32_t GetWidth() override { return textureWidth; };
		uint32_t GetHeight() override { return textureHeight; };
	private :
		void UploadMipmap(VulkanCommandBuffer cmd);
		VulkanImage vulkanImage;
		VulkanImageView vulkanImageView;
		uint32_t textureWidth;
		uint32_t textureHeight;
		VkImageUsageFlags vkImageUsage;
	};
}