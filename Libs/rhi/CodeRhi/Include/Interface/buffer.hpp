#pragma once
#include "device.hpp"
#include "enums/buffer_usage.hpp"
#include "Interface/upload_context.hpp"
namespace rhi {
	class VulkanBuffer;
	class CommandBuffer;
	class Buffer {
	public :
		virtual ~Buffer() = default;
		
		virtual void CreateGpu(Device* device, BufferUsage usage, size_t size) = 0;
		virtual void CreateGpu(Device* device, UploadContext* uploadContext, BufferUsage usage, void* data, size_t size) = 0;
		virtual void CreateGpu(Device* device, CommandBuffer* commandBuffer, BufferUsage usage, void* data, size_t size) = 0;
		virtual void ReallocGpu(Device* device, UploadContext* uploadContext, size_t size) = 0;
		virtual void ReallocGpu(Device* device, CommandBuffer* commandBuffer, size_t size) = 0;
		virtual void ReallocNoCopyGpu(Device* device, size_t size) = 0;
		virtual void CopyGpu(Device* device, UploadContext* uploadContext, Buffer* otherBuffer, size_t copySize) = 0;
		virtual void CopyGpu(Device* device, CommandBuffer* commandBuffer, Buffer* otherBuffer, size_t copySize) = 0;
		virtual void* GetBufferData() = 0;
		virtual void CreateCpu(Device* device, BufferUsage usage, void* data, size_t size) = 0;
		virtual void ReallocCpu(Device* device, size_t size) = 0;
		virtual void ReallocNoCopyCpu(Device* device, size_t size) = 0;
		virtual void CopyCpu(Device* device, Buffer* otherBuffer, size_t copySize) =0;
		virtual void ReadCpu(void* data, size_t readSize, size_t offset) =0;
		virtual void SetCpu(void* data, size_t setSize, size_t offset) =0;
		virtual void Destroy(Device* device) = 0;
		virtual size_t GetBufferSize() = 0;
		virtual bool IsCpuBuffer() = 0;
		virtual bool IsGpuBuffer() = 0;
		virtual void TransitionBuffer(CommandBuffer* commandBuffer, BufferState dstState) = 0;
		virtual uint64_t GetDeviceAddress(Device* device) = 0;
		virtual VulkanBuffer& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanBuffer"); }
	protected :
		BufferUsage usage;
	};
}