#pragma once
#include "vulkan/vulkan.h"
#include "IVulkan/vulkan_device.hpp"
#include "Interface/buffer.hpp"
#include "IVulkan/vulkan_upload_context.hpp"
#include "IVulkan/enums/vulkan_buffer_usage.hpp"
#include <string>

namespace rhi {
	class VulkanBuffer : public Buffer {
	public :
		
		
		void CreateVk(VulkanDevice& device, VkDeviceSize bufferSize, VkBufferUsageFlags usage, VmaAllocationCreateFlags flags, VkMemoryPropertyFlags requiredFlags);
		void CreateGpuVk(VulkanDevice& device, VkBufferUsageFlags usage, size_t size);
		void CreateGpuVk(VulkanDevice& device, VulkanUploadContext vulkanUploadContext,VkBufferUsageFlags usage, void* data, size_t size);
		void CreateGpuVk(VulkanDevice& device, VulkanCommandBuffer& vulkanCommandBuffer, VkBufferUsageFlags usage, void* data, size_t size);
		void ReallocGpuVk(VulkanDevice& vulkanDevice, VulkanUploadContext vulkanUploadContext, size_t size);
		void ReallocNoCopyGpuVk(VulkanDevice& vulkanDevice, size_t size);
		void CreateCpuVk(VulkanDevice& vulkanDevice, VkBufferUsageFlags usage, void* data, size_t size);
		void CopyGpuVk(VulkanDevice& vulkanDevice, VulkanUploadContext& uploadContext, VkBuffer dstBuffer, size_t copySize);
		void CopyGpuVk(VulkanDevice& vulkanDevice, VulkanCommandBuffer& vulkanCommandBuffer, VkBuffer dstBuffer, size_t copySize);
		void ReallocCpuVk(VulkanDevice& vulkanDeivce, size_t size);
		void ReallocNoCopyCpuVk(VulkanDevice& device, size_t size);
		void CopyCpuVk(VulkanDevice& vulkanDeivce, VulkanBuffer& other, size_t copySize);

		void* GetBufferData() override;
		void TransitionBufferVk(VulkanCommandBuffer& vulkanCommandBuffer, VulkanBufferState vulkanBufferState);

		void CreateGpu(Device* device, BufferUsage usage, size_t size) override;
		void CreateGpu(Device* device, UploadContext* uploadContext, BufferUsage usage, void* data, size_t size) override;
		void CreateGpu(Device* device, CommandBuffer* commandBuffer, BufferUsage usage, void* data, size_t size) override;
		void ReallocGpu(Device* device, UploadContext* uploadContext, size_t size) override;
		void ReallocNoCopyGpu(Device* device, size_t size) override;
		void CopyGpu(Device* device, UploadContext* uploadContext, Buffer* otherBuffer, size_t copySize) override;
		void CopyGpu(Device* device, CommandBuffer* commandBuffer, Buffer* otherBuffer, size_t copySize) override;
		
		void CreateCpu(Device* device, BufferUsage usage, void* data, size_t size) override;
		void ReallocCpu(Device* device, size_t size) override;
		void ReallocNoCopyCpu(Device* device, size_t size) override;
		void CopyCpu(Device* device, Buffer* otherBuffer, size_t copySize) override;

		void ReadCpu(void* data, size_t readSize, size_t offset) override;
		void SetCpu(void* data, size_t setSize, size_t offset) override;
		uint64_t GetDeviceAddress(Device* device) override;
		void TransitionBuffer(CommandBuffer* commandBuffer, BufferState dstState) override;
		void Destroy(Device* device) override;

		void CheckCpu(std::string operationName);
		void CheckGpu(std::string operationName);
		
		bool IsCpuBuffer() override { return (vkRequiredFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT); };
		bool IsGpuBuffer() override { return !IsCpuBuffer(); } ;

		
		void Destroy(VulkanDevice& device);
		size_t GetBufferSize() override { return bufferSize; }
		VkBuffer GetBuffer() const { return buffer; }
		VulkanBuffer& API_VULKAN() override { return *(this); }
	private :
		
		void* mappedPtr = nullptr;
		VkBufferUsageFlags vkBufferUsage;
		VkMemoryPropertyFlags vkFlags;
		VkMemoryPropertyFlags vkRequiredFlags;

		VkBuffer buffer = VK_NULL_HANDLE;
		VmaAllocation allocation = VK_NULL_HANDLE;
		VkDeviceSize bufferSize;
		VulkanBufferState currentBufferState;
	};
}