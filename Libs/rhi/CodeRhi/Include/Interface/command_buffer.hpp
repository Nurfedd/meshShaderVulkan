#pragma once
#include <exception>
#include "enums/input_fomat.hpp"

namespace rhi {
	class VulkanCommandBuffer;
	class MemoryBarrier;
	class BufferBarrier;
	class ImageBarrier;
	class Buffer;
	class Pipeline;
	struct BarriersDependency {
		MemoryBarrier* memoryBarriers;
		BufferBarrier* bufferBarriers;
		ImageBarrier* imageBarriers;

		uint32_t memoryBarrierCount = 0;
		uint32_t bufferBarrierCount = 0;
		uint32_t imageBarrierCount = 0;
	};
	class CommandBuffer {
	public :
		virtual ~CommandBuffer() = default;
		virtual void Reset() = 0;
		virtual void Begin() = 0;
		virtual void SetViewport(uint32_t x,uint32_t y,uint32_t width, uint32_t height) = 0;
		virtual void SetScissor(int32_t offsetX,int32_t offsetY,uint32_t width, uint32_t height) = 0;
		virtual void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) = 0;
		virtual void DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t firstInstance) = 0;
		virtual void DrawIndexedIndirect(Buffer* indirectBuffer, uint32_t drawCount, size_t offset, size_t stride) = 0;
		virtual void BindVertexBuffers(Buffer** buffers,uint32_t firstBinding,uint32_t count) = 0;
		virtual void BindIndexBuffer(Buffer* indexBuffer, IndexType indexType) = 0;
		virtual void Dispatch(uint32_t groupX, uint32_t groupY, uint32_t groupZ) = 0;
		virtual void End() = 0;
		virtual void PipelineBarrier(BarriersDependency dependecy) = 0;
		virtual VulkanCommandBuffer& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanCommandBuffer"); }
	protected :
	};
}