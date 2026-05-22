#include "IVulkan/vulkan_queue_ownership.hpp"

void QueueOwnership::AddFamily(uint32_t family) {
	familyIndices.insert(family);
}

VkSharingMode QueueOwnership::GetSharingMode() {
	if (familyIndices.size() == 0)
		return VK_SHARING_MODE_EXCLUSIVE;
	uint32_t prev = -1;

	for (uint32_t family : familyIndices) {
		if (prev == -1) {
			prev = family;
			continue;
		}
		if (prev != family)
			return VK_SHARING_MODE_CONCURRENT;
	}
	return VK_SHARING_MODE_EXCLUSIVE;
}