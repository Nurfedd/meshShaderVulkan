#pragma once
#include "Interface/fence.hpp"
#include "vulkan/vulkan.h"
namespace rhi {
	class VulkanFence : public Fence {
	public :
		void CreateVk(VulkanDevice& vulkanDevice, VkFenceCreateFlags flags);
		void Create(Device* device) override;
		void Destroy(Device* device) override;
		void DestroyVk(VulkanDevice& vulkanDevice);
		VulkanFence& API_VULKAN() override { return *(this); }
		VkFence GetFence() const { return fence; }
	private :
		VkFence fence;
	};
}
