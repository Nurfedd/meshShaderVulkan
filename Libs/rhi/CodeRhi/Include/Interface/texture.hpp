#pragma once
#include "device.hpp"
#include "enums/format.hpp"
#include "enums/image_type.hpp"
#include "upload_context.hpp"
#include "command_buffer.hpp"
#include "enums/synchronization_stage.hpp"
#include <exception>
namespace rhi {
	class VulkanTexture;
	struct TextureCreateInfo {
		uint32_t width;
		uint32_t height;
		Format textureFormat;
		ImageUsage imageUsage;
		uint32_t mipCount;
		ViewType viewType;
	};
	class Texture {
	public :
		virtual ~Texture() = default;
		virtual void Create(Device* device, TextureCreateInfo createInfo) = 0;
		virtual void Resize(Device*, uint32_t width, uint32_t height) = 0;
		virtual void Upload(Device* device,UploadContext* uploadContext,void* pixels, uint32_t channels) = 0;
		virtual void Upload(Device* device, CommandBuffer* commandBuffer, void* pixels, uint32_t channels) = 0;
		virtual bool MakeImageReadableInShader(CommandBuffer* commandBuffer,PipelineStageFlags dstStage,AccessFlags accessFlags) = 0;
		virtual bool CanBeUsedInShader() = 0;
		virtual void Destroy(Device* device) = 0;
		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;
		virtual VulkanTexture& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanFrameBuffer"); }
	};
}