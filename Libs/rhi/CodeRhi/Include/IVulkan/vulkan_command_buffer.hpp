#pragma once
#include "Interface/command_buffer.hpp"
#include "IVulkan/vulkan_barriers.hpp"
#include "vulkan/vulkan.h"

namespace rhi {
	class VulkanPipeline;
	struct BarriersDependencyVk {
		VulkanMemoryBarrier* memoryBarriers;
		VulkanBufferBarrier* bufferBarriers;
		VulkanImageBarrier* imageBarriers;

		uint32_t memoryBarrierCount = 0;
		uint32_t bufferBarrierCount = 0;
		uint32_t imageBarrierCount = 0;
	};
	class VulkanCommandBuffer : public CommandBuffer {
	public :
		VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
		void ResetVk(VkCommandBufferResetFlags flags);
		void Reset() override;
		void BeginVk(VkCommandBufferUsageFlags usage);
		void Begin() override;
		void SetViewport(uint32_t x, uint32_t y,uint32_t width, uint32_t height) override;
		void SetScissor(int32_t offsetX, int32_t offsetY,uint32_t width, uint32_t height) override;
		void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) override;
		void DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t firstInstance) override;
		void DrawIndexedIndirect(Buffer* indirectBuffer, uint32_t drawCount, size_t offset, size_t stride) override;
		
		void BindVertexBuffers(Buffer** buffers, uint32_t firstBinding, uint32_t count) override;
		void BindIndexBuffer(Buffer* indexBuffer, IndexType indexType) override;
		void Dispatch(uint32_t groupX, uint32_t groupY, uint32_t groupZ) override;
		void PipelineBarrierVk(BarriersDependencyVk vkBarriers);
		void PipelineBarrier(BarriersDependency dependecy) override;
		void End() override;
		VulkanCommandBuffer& API_VULKAN() { return *(this); }
	private:
		
	};
}