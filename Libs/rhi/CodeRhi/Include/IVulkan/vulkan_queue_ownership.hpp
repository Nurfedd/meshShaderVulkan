#pragma once
#include <unordered_set>
#include "vulkan/vulkan.h"

class QueueOwnership {
public :
	void AddFamily(uint32_t family);
	std::unordered_set<uint32_t> GetIndices() { return familyIndices; }
	VkSharingMode GetSharingMode();
private :
	std::unordered_set<uint32_t> familyIndices;
};