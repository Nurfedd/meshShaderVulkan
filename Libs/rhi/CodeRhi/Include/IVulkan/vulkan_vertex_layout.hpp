#pragma once
#include "Interface/vertex_layout.hpp"
#include "vulkan/vulkan.h"
#include <vector>
namespace rhi {
	void GetVulkanVertexLayoutObjects(const VertexLayout& vertexLayout, std::vector<VkVertexInputAttributeDescription>& inputDescriptions,VkVertexInputBindingDescription& bindingDescription);
	
}