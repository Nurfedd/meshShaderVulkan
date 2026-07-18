#pragma once
#include "Interface/fence.hpp"
#include "vulkan/vulkan.h"
namespace rhi {
	class VulkanFence : public Fence {
	public :
		void CreateVk(VulkanDevice& vulkanDevice, VkFenceCreateFlags flags);
		bool IsReadyVk(VulkanDevice& vulkanDevice);
		void DestroyVk(VulkanDevice& vulkanDevice);

		void Create(Device* device,bool initialReady) override;
		bool IsReady(Device* device) override;
		void Destroy(Device* device) override;
		VulkanFence& API_VULKAN() override { return *(this); }
		VkFence GetFence() const { return fence; }
	private :
		VkFence fence;
	};
}
