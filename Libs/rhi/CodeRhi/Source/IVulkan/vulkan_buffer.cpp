#include "IVulkan/vulkan_buffer.hpp"
#include "Volk/volk.h"
#include "IVulkan/enums/vulkan_buffer_usage.hpp"
#include "IVulkan/enums/vulkan_synchronization_stage.hpp"
#include "IVulkan/vulkan_barriers.hpp" 
namespace rhi {
    // VULKAN IMPL
	void VulkanBuffer::CreateVk(VulkanDevice& device, VkDeviceSize size, VkBufferUsageFlags usage, VmaAllocationCreateFlags flags, VkMemoryPropertyFlags requiredFlags) {
        bufferSize = size;
        vkBufferUsage = usage;
        vkFlags = flags;
        vkRequiredFlags = requiredFlags;

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = bufferSize;
        bufferInfo.usage = vkBufferUsage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo allocInfo{};
        allocInfo.flags = flags;
        allocInfo.requiredFlags = requiredFlags;
        allocInfo.preferredFlags = 0;
        
        VkResult res = vmaCreateBuffer(
            device.GetAllocator(),
            &bufferInfo,
            &allocInfo,
            &buffer,
            &allocation,
            nullptr
        );
	}

    void VulkanBuffer::CreateGpuVk(VulkanDevice& vulkanDevice, VkBufferUsageFlags bufferUsage, size_t size) {
        CreateVk(vulkanDevice, size, bufferUsage, 0, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    }
    void VulkanBuffer::CreateGpuVk(VulkanDevice& vulkanDevice, VulkanUploadContext vulkanUploadContext, VkBufferUsageFlags bufferUsage, void* data, size_t size) {
        VkBufferUsageFlags bufferUsageFlags = bufferUsage | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        CreateVk(vulkanDevice, size, bufferUsageFlags, 0, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        VulkanBuffer stagingBuffer;
        stagingBuffer.CreateCpuVk(vulkanDevice, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, data, size);
        CopyGpuVk(vulkanDevice, vulkanUploadContext, stagingBuffer.GetBuffer(), size);
        stagingBuffer.Destroy(vulkanDevice);
    }

    void VulkanBuffer::CreateGpuVk(VulkanDevice& vulkanDevice, VulkanCommandBuffer& vulkanCommandBuffer, VkBufferUsageFlags bufferUsage, void* data, size_t size) {
        VkBufferUsageFlags bufferUsageFlags = bufferUsage | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        CreateVk(vulkanDevice, size, bufferUsageFlags, 0, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        VulkanBuffer stagingBuffer;
        stagingBuffer.CreateCpuVk(vulkanDevice, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, data, size);
        CopyGpuVk(vulkanDevice, vulkanCommandBuffer, stagingBuffer.GetBuffer(), size);
        stagingBuffer.Destroy(vulkanDevice);
    }

    void VulkanBuffer::ReallocGpuVk(VulkanDevice& vulkanDevice, VulkanUploadContext vulkanUploadContext, size_t newSize) {
        CheckGpu("Realloc");
        VmaAllocation oldAllocation = allocation;
        size_t oldSize = bufferSize;
        
        VkBuffer oldBuffer = buffer;
        CreateGpuVk(vulkanDevice, vkBufferUsage, newSize);
        size_t copySize = std::min(oldSize, newSize);

        CopyGpuVk(vulkanDevice, vulkanUploadContext, oldBuffer, copySize);
        vmaDestroyBuffer(vulkanDevice.GetAllocator(), oldBuffer, oldAllocation);
    }

    void VulkanBuffer::ReallocGpuVk(VulkanDevice& vulkanDevice, VulkanCommandBuffer& vulkanCommandBuffer, size_t newSize) {
        CheckGpu("Realloc");
        VmaAllocation oldAllocation = allocation;
        size_t oldSize = bufferSize;

        VkBuffer oldBuffer = buffer;
        CreateGpuVk(vulkanDevice, vkBufferUsage, newSize);
        size_t copySize = std::min(oldSize, newSize);

        CopyGpuVk(vulkanDevice, vulkanCommandBuffer, oldBuffer, copySize);
        vmaDestroyBuffer(vulkanDevice.GetAllocator(), oldBuffer, oldAllocation);
    }

    void VulkanBuffer::ReallocNoCopyGpuVk(VulkanDevice& vulkanDevice, size_t size) {
        CheckGpu("Realloc");
        Destroy(vulkanDevice);
        CreateGpuVk(vulkanDevice, vkBufferUsage, size);
    }

    void VulkanBuffer::CopyGpuVk(VulkanDevice& vulkanDevice, VulkanUploadContext& uploadContext, VkBuffer sourceBuffer, size_t copySize) {
        CheckGpu("Copy");
        uploadContext.Upload(vulkanDevice, [&](VulkanCommandBuffer commandBuffer) {
            CopyGpuVk(vulkanDevice, commandBuffer, sourceBuffer, copySize);
            });
    }
    void VulkanBuffer::CopyGpuVk(VulkanDevice& vulkanDevice, VulkanCommandBuffer& vulkanCommandBuffer, VkBuffer sourceBuffer, size_t copySize) {
        VkBufferCopy copy{};
        copy.size = copySize;
        VkCommandBuffer cmd = vulkanCommandBuffer.commandBuffer;

        vkCmdCopyBuffer(cmd, sourceBuffer, buffer, 1, &copy);
        VulkanBarrierInfo barrierInfo = VulkanUsageToBarrier(vkBufferUsage);

        VulkanBufferBarrier bufferBarrier;
        bufferBarrier.SetStages(
            VK_PIPELINE_STAGE_2_TRANSFER_BIT,
            VK_ACCESS_2_TRANSFER_WRITE_BIT,
            barrierInfo.stage,
            barrierInfo.access
        );
        bufferBarrier.buffer = buffer;
        bufferBarrier.offset = 0;
        bufferBarrier.size = VK_WHOLE_SIZE;

        BarriersDependencyVk dependency;
        dependency.bufferBarrierCount = 1;
        dependency.bufferBarriers = &bufferBarrier;

        vulkanCommandBuffer.PipelineBarrierVk(dependency);
    }
    void VulkanBuffer::CreateCpuVk(VulkanDevice& vulkanDevice, VkBufferUsageFlags bufferUsage, void* data, size_t size) {
        CreateVk(vulkanDevice, size, bufferUsage, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        vmaMapMemory(vulkanDevice.GetAllocator(), allocation, &mappedPtr);
        SetCpu(data, size, 0);
    }

    void VulkanBuffer::ReallocCpuVk(VulkanDevice& vulkanDevice, size_t newSize) {
        CheckCpu("Realloc");
        void* newData = malloc(newSize);
        size_t copySize = std::min(bufferSize, newSize);
        ReadCpu(newData, copySize, 0);
        Destroy(vulkanDevice);
        CreateCpuVk(vulkanDevice, vkBufferUsage, nullptr, newSize);
        SetCpu(newData, copySize, 0);
        free(newData);
    }

    void VulkanBuffer::ReallocNoCopyCpuVk(VulkanDevice& vulkanDevice, size_t newSize) {
        CheckCpu("Realloc");
        Destroy(vulkanDevice);
        CreateCpuVk(vulkanDevice, vkBufferUsage, nullptr, newSize);
    }

    void VulkanBuffer::CopyCpuVk(VulkanDevice& vulkanDevice, VulkanBuffer& other, size_t copySize) {
        CheckCpu("Copy");
        other.CheckCpu("Copy");
        copySize = std::min({ copySize,bufferSize,other.GetBufferSize() });

        void* readData = malloc(copySize);
        other.ReadCpu(readData, copySize, 0);
        SetCpu(readData, copySize, 0);
        free(readData);
    }

    void* VulkanBuffer::GetBufferData() {
        return mappedPtr;
    }
    void VulkanBuffer::TransitionBufferVk(VulkanCommandBuffer& vulkanCommandBuffer, VulkanBufferState dstBufferState) {
        VulkanBufferBarrier bufferBarrier;
        bufferBarrier.SetStages(
            currentBufferState.pipelineStage,
            currentBufferState.accessFlags,
            dstBufferState.pipelineStage,
            dstBufferState.accessFlags
        );
        bufferBarrier.buffer = buffer;
        bufferBarrier.offset = 0;
        bufferBarrier.size = VK_WHOLE_SIZE;

        BarriersDependencyVk dependency;
        dependency.bufferBarrierCount = 1;
        dependency.memoryBarrierCount = 0;
        dependency.imageBarrierCount = 0;
        dependency.bufferBarriers = &bufferBarrier;
        vulkanCommandBuffer.PipelineBarrierVk(dependency);

        currentBufferState = dstBufferState; // update new state
    }

    // RHI IMPL
    void VulkanBuffer::CreateGpu(Device* device, BufferUsage usage, size_t size) {
        CreateGpuVk(device->API_VULKAN(), ToVulkanBufferUsage(usage), size);
    }
    void VulkanBuffer::CreateGpu(Device* device, UploadContext* uploadContext, BufferUsage usage, void* data, size_t size) {
        CreateGpuVk(device->API_VULKAN(), uploadContext->API_VULKAN(), ToVulkanBufferUsage(usage), data, size);
    }

    void VulkanBuffer::CreateGpu(Device* device, CommandBuffer* commandBuffer, BufferUsage usage, void* data, size_t size) {
        CreateGpuVk(device->API_VULKAN(), commandBuffer->API_VULKAN(), ToVulkanBufferUsage(usage), data, size);
    }

    void VulkanBuffer::ReallocGpu(Device* device, UploadContext* uploadContext, size_t size) {
        ReallocGpuVk(device->API_VULKAN(), uploadContext->API_VULKAN(), size);
    }

    void VulkanBuffer::ReallocGpu(Device* device, CommandBuffer* commandBuffer, size_t size) {
        ReallocGpuVk(device->API_VULKAN(), commandBuffer->API_VULKAN(), size);
    }

    void VulkanBuffer::ReallocNoCopyGpu(Device* device, size_t size) {
        ReallocNoCopyGpuVk(device->API_VULKAN(), size);
    }

    void VulkanBuffer::CopyGpu(Device* device, UploadContext* uploadContext, Buffer* otherBuffer, size_t copySize) {
        CopyGpuVk(device->API_VULKAN(), uploadContext->API_VULKAN(), otherBuffer->API_VULKAN().GetBuffer(), copySize);
    }

    void VulkanBuffer::CopyGpu(Device* device, CommandBuffer* commandBuffer, Buffer* otherBuffer, size_t copySize) {
        CopyGpuVk(device->API_VULKAN(), commandBuffer->API_VULKAN(), otherBuffer->API_VULKAN().GetBuffer(), copySize);
    }

    void VulkanBuffer::CreateCpu(Device* device, BufferUsage usage, void* data, size_t size) {
        CreateCpuVk(device->API_VULKAN(), ToVulkanBufferUsage(usage), data, size);
    }

    void VulkanBuffer::ReallocCpu(Device* device, size_t size) {
        ReallocCpuVk(device->API_VULKAN(), size);
    }

    void VulkanBuffer::ReallocNoCopyCpu(Device* device, size_t size) {
        ReallocNoCopyCpuVk(device->API_VULKAN(), size);
    }

    void VulkanBuffer::CopyCpu(Device* device, Buffer* otherBuffer, size_t copySize) {
        CopyCpuVk(device->API_VULKAN(), otherBuffer->API_VULKAN(), copySize);
    }

    void VulkanBuffer::ReadCpu(void* data, size_t readSize, size_t offset) {
        CheckCpu("Read");
        if (data == nullptr)
            return;
        if (readSize + offset > bufferSize)
            return;
        void* dst = static_cast<char*>(mappedPtr) + offset;
        memcpy(data, dst, readSize);
    }
    void VulkanBuffer::SetCpu(void* data, size_t setSize, size_t offset) {
        CheckCpu("Set");
        if (data == nullptr)
            return;
        if (setSize + offset > bufferSize)
            return;
        void* dst = static_cast<char*>(mappedPtr) + offset;
        memcpy(dst, data, setSize);
    }

    

    void VulkanBuffer::CheckCpu(std::string operationName) {
        if (!IsCpuBuffer()) {
            std::string message = "Operation : " + operationName + " has to be a CPU buffer";
            throw std::exception(message.c_str());
        }
    }

    void VulkanBuffer::CheckGpu(std::string operationName) {
        if (!IsGpuBuffer()) {
            std::string message = "Operation : " + operationName + " has to be a GPU buffer";
            throw std::exception(message.c_str());
        }
    }

    uint64_t VulkanBuffer::GetDeviceAddress(Device* device) {
        VkBufferDeviceAddressInfo addressInfo{};
        addressInfo.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
        addressInfo.buffer = buffer;
        return vkGetBufferDeviceAddress(device->API_VULKAN(), &addressInfo);
    }
    void VulkanBuffer::TransitionBuffer(CommandBuffer* commandBuffer, BufferState dstState) {
        TransitionBufferVk(commandBuffer->API_VULKAN(), ToVulkanBufferState(dstState));
    }
    void VulkanBuffer::Destroy(Device* device) {
        Destroy(device->API_VULKAN());
    }
	void VulkanBuffer::Destroy(VulkanDevice& device) {
        if (mappedPtr != nullptr) {
            vmaUnmapMemory(device.GetAllocator(), allocation);
        }
		if (buffer != VK_NULL_HANDLE) {
			vmaDestroyBuffer(device.GetAllocator(), buffer, allocation);
			buffer = VK_NULL_HANDLE;
			allocation = VK_NULL_HANDLE;
		}
	}
}