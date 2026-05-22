#include "IVulkan/vulkan_dynamic_renderer.hpp"
#include "IVulkan/vulkan_texture.hpp"
#include "IVulkan/vulkan_command_buffer.hpp"
#include "Volk/volk.h"

namespace rhi {
	
	void VulkanDynamicRenderer::Begin(CommandBuffer* commandBuffer, BeginRenderingInfo beginRenderingInfo) {
		VulkanCommandBuffer& vulkanCommandBuffer = commandBuffer->API_VULKAN();

		std::vector<VkRenderingAttachmentInfo> renderingColorAttachments(beginRenderingInfo.colorAttachmentCount);
		VkRenderingAttachmentInfo renderingDepthAttachment{};
		bool hasDepth = false;
		// images barriers
		for (uint32_t i = 0; i < renderingColorAttachments.size(); i++) {
			VulkanTexture& vulkanTexture = beginRenderingInfo.colorAttachments[i].texture->API_VULKAN();
			
			vulkanTexture.GetImage()->TransitionImage(
				vulkanCommandBuffer, 
				VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 
				VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT, 
				VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
				VK_IMAGE_ASPECT_COLOR_BIT
			);

			VkRenderingAttachmentInfo info{};
			info.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;

			ClearValue attachmentClearValue = beginRenderingInfo.colorAttachments[i].clearValue;
			VkClearValue clearValue{};
			clearValue.color = {attachmentClearValue.r,attachmentClearValue.g ,attachmentClearValue.b ,attachmentClearValue.a };
			
			info.clearValue = clearValue;
			bool shouldClear = beginRenderingInfo.colorAttachments[i].clearBuffer;
			info.loadOp = (shouldClear) ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
			info.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			info.imageView = vulkanTexture.GetImageView()->GetVkImageView();
			info.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			renderingColorAttachments[i] = info;
		}

		if (beginRenderingInfo.depthAttachment) {
			VulkanTexture& vulkanDepthTexture = beginRenderingInfo.depthAttachment->texture->API_VULKAN();
			vulkanDepthTexture.GetImage()->TransitionImage(
				vulkanCommandBuffer,
				VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
				VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT,
				VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
				VK_IMAGE_ASPECT_DEPTH_BIT);
			hasDepth = true;

			renderingDepthAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;

			VkClearValue depthClear{};
			depthClear.depthStencil.depth = beginRenderingInfo.depthAttachment->clearDepth;
			depthClear.depthStencil.stencil = 0;

			renderingDepthAttachment.clearValue = depthClear;
			renderingDepthAttachment.imageView = vulkanDepthTexture.GetImageView()->GetVkImageView();
			renderingDepthAttachment.imageLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
			renderingDepthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			renderingDepthAttachment.loadOp = (beginRenderingInfo.depthAttachment->clearBuffer) ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
		}
		VkExtent2D extent;
		extent.width = beginRenderingInfo.width;
		extent.height = beginRenderingInfo.height;

		VkRenderingInfo renderingInfo{};
		renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
		renderingInfo.renderArea.offset = { 0, 0 };
		renderingInfo.renderArea.extent = extent;
		renderingInfo.layerCount = 1;
		renderingInfo.colorAttachmentCount = renderingColorAttachments.size();
		renderingInfo.pColorAttachments = renderingColorAttachments.data();
		renderingInfo.pDepthAttachment = (hasDepth) ? &renderingDepthAttachment : nullptr;
		renderingInfo.pStencilAttachment = nullptr;
		
		vkCmdBeginRendering(vulkanCommandBuffer.commandBuffer, &renderingInfo);
	}

	void VulkanDynamicRenderer::End(CommandBuffer* commandBuffer) {
		VulkanCommandBuffer& vulkanCommandBuffer = commandBuffer->API_VULKAN();
		vkCmdEndRendering(vulkanCommandBuffer.commandBuffer);
	}
}