#pragma once
#include "command_buffer.hpp"
#include "texture.hpp"
#include <vector>
#include <exception>
namespace rhi {
	class VulkanDynamicRenderer;
	struct ClearValue {
		ClearValue() = default;
		ClearValue(float r, float g, float b, float a) {
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}
		float r = 0;
		float g = 0;
		float b = 0;
		float a = 0;
	};

	struct Attachment {
		Texture* texture = nullptr;
		bool clearBuffer = true;
		ClearValue clearValue;
		float clearDepth;
	};

	struct BeginRenderingInfo {
		Attachment* colorAttachments;
		Attachment* depthAttachment = nullptr;

		uint32_t colorAttachmentCount;
		uint32_t width;
		uint32_t height;
	};
	class DynamicRenderer {
	public:
		virtual ~DynamicRenderer() = default;
		
		virtual void Begin(CommandBuffer* commandBuffer, BeginRenderingInfo beginRenderingInfo) = 0;
		virtual void End(CommandBuffer* commandBuffer) = 0;
		virtual VulkanDynamicRenderer& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanDynamicRenderer"); };
	};
}