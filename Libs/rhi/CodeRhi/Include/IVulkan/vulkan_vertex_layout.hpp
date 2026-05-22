#pragma once
#include "Interface/vertex_layout.hpp"
#include "vulkan/vulkan.h"
#include <vector>
namespace rhi {
	class VulkanVertexLayout : public VertexLayout {
	public :
		void Create(VertexInput* vertexInputs, uint32_t inputCount, uint32_t _binding, uint32_t _stride) override;
		VulkanVertexLayout& API_VULKAN() override { return *(this); }
		std::vector<VkVertexInputAttributeDescription> GetInputDescriptions() const { return inputDescriptions; }
		VkVertexInputBindingDescription GetBindingDescription() const { return bindingDescription; }
	private :
		std::vector<VkVertexInputAttributeDescription> inputDescriptions;
		VkVertexInputBindingDescription bindingDescription;
	};
}