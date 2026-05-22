#include "IVulkan/vulkan_command_buffer.hpp"
#include "IVulkan/vulkan_buffer.hpp"
#include "IVulkan/enums/vulkan_input_format.hpp"
#include "Volk/volk.h"
#include "IVulkan/vulkan_pipeline.hpp"
namespace rhi {

	void VulkanCommandBuffer::ResetVk(VkCommandBufferResetFlags flags) {
		vkResetCommandBuffer(commandBuffer, flags);
	}

	void VulkanCommandBuffer::BeginVk(VkCommandBufferUsageFlags usage) {
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = usage;
		vkBeginCommandBuffer(commandBuffer, &beginInfo);
	}

	void VulkanCommandBuffer::PipelineBarrierVk(BarriersDependencyVk vkBarriers) {
		VkDependencyInfo dependencyInfo{};
		dependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;

		std::vector<VkMemoryBarrier2> memoryBarriers(vkBarriers.memoryBarrierCount);
		std::vector<VkBufferMemoryBarrier2> bufferBarriers(vkBarriers.bufferBarrierCount);
		std::vector<VkImageMemoryBarrier2> imageBarriers(vkBarriers.imageBarrierCount);

		for (uint32_t i = 0; i < memoryBarriers.size(); i++)
			memoryBarriers[i] = vkBarriers.memoryBarriers[i].GetBarrier();

		for (uint32_t i = 0; i < bufferBarriers.size(); i++)
			bufferBarriers[i] = vkBarriers.bufferBarriers[i].GetBarrier();

		for (uint32_t i = 0; i < imageBarriers.size(); i++)
			imageBarriers[i] = vkBarriers.imageBarriers[i].GetBarrier();

		dependencyInfo.pMemoryBarriers = memoryBarriers.data();
		dependencyInfo.pBufferMemoryBarriers = bufferBarriers.data();
		dependencyInfo.pImageMemoryBarriers = imageBarriers.data();

		dependencyInfo.memoryBarrierCount = memoryBarriers.size();
		dependencyInfo.bufferMemoryBarrierCount = bufferBarriers.size();
		dependencyInfo.imageMemoryBarrierCount = imageBarriers.size();

		vkCmdPipelineBarrier2(commandBuffer, &dependencyInfo);
	}

	void VulkanCommandBuffer::Reset() {
		ResetVk(0);
		
	}

	void VulkanCommandBuffer::Begin() {
		BeginVk(0);
	}
	void VulkanCommandBuffer::SetViewport(uint32_t x, uint32_t y,uint32_t width, uint32_t height) {
		VkViewport viewport{};
		viewport.x = x;
		viewport.y = y;
		viewport.width = static_cast<float>(width);
		viewport.height = static_cast<float>(height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	}
	void VulkanCommandBuffer::SetScissor(int32_t offsetX, int32_t offsetY,uint32_t width, uint32_t height) {
		VkRect2D scissor{};
		scissor.offset = { offsetX,offsetY };
		scissor.extent = { width,height };
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}
	void VulkanCommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
		vkCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void VulkanCommandBuffer::DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t firstInstance) {
		vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, 0, firstInstance);
	}

	void VulkanCommandBuffer::DrawIndexedIndirect(Buffer* indirectBuffer, uint32_t drawCount, size_t offset, size_t stride) {
		VkBuffer buffer = indirectBuffer->API_VULKAN().GetBuffer();
		vkCmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
	}
	
	void VulkanCommandBuffer::BindVertexBuffers(Buffer** buffers, uint32_t firstBinding, uint32_t count) {
		std::vector<VkDeviceSize> offsets(count);
		std::fill(offsets.begin(), offsets.end(), 0);

		std::vector<VkBuffer> vkBuffers(count);
		for (uint32_t i = 0; i < count;i++) {
			vkBuffers[i] = buffers[i]->API_VULKAN().GetBuffer();
		}
		vkCmdBindVertexBuffers(commandBuffer, firstBinding, count, vkBuffers.data(), offsets.data());
	}

	void VulkanCommandBuffer::BindIndexBuffer(Buffer* indexBuffer, IndexType indexType) {
		VkBuffer buffer = indexBuffer->API_VULKAN().GetBuffer();
		vkCmdBindIndexBuffer(commandBuffer, buffer, 0, ToVulkanIndexType(indexType));
	}

	void VulkanCommandBuffer::Dispatch(uint32_t groupX, uint32_t groupY, uint32_t groupZ) {
		vkCmdDispatch(commandBuffer, groupX, groupY, groupZ);
	}
 
	void VulkanCommandBuffer::PipelineBarrier(BarriersDependency dependency) {
		BarriersDependencyVk vkDependency;
		vkDependency.memoryBarrierCount = dependency.memoryBarrierCount;
		vkDependency.bufferBarrierCount = dependency.bufferBarrierCount;
		vkDependency.imageBarrierCount = dependency.imageBarrierCount;

		std::vector<VulkanMemoryBarrier> memoryBarriers (dependency.memoryBarrierCount);
		std::vector<VulkanBufferBarrier> bufferBarriers(dependency.bufferBarrierCount);
		std::vector<VulkanImageBarrier> imageBarriers(dependency.imageBarrierCount);

		for (uint32_t i = 0; i < memoryBarriers.size(); i++)
			memoryBarriers[i] = VulkanMemoryBarrier(dependency.memoryBarriers[i]);

		for (uint32_t i = 0; i < bufferBarriers.size(); i++)
			bufferBarriers[i] = VulkanBufferBarrier(dependency.bufferBarriers[i]);

		for (uint32_t i = 0; i < imageBarriers.size(); i++)
			imageBarriers[i] = VulkanImageBarrier(dependency.imageBarriers[i]);

		vkDependency.memoryBarriers = memoryBarriers.data();
		vkDependency.bufferBarriers = bufferBarriers.data();
		vkDependency.imageBarriers = imageBarriers.data();

		PipelineBarrierVk(vkDependency);
	}


	

	void VulkanCommandBuffer::End() {
		vkEndCommandBuffer(commandBuffer);
	}
}