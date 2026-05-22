#include "IVulkan/vulkan_vertex_layout.hpp"
#include "IVulkan/enums/vulkan_input_format.hpp"
namespace rhi {
	void VulkanVertexLayout::Create(VertexInput* vertexInputs, uint32_t inputCount, uint32_t _binding, uint32_t _stride) {
		VertexLayout::Create(vertexInputs, inputCount, _binding, _stride);
		inputDescriptions.resize(inputCount);
		for (int i = 0; i < inputCount; i++) {
			VkVertexInputAttributeDescription newAttribute{};
			VertexInput vertexInput = vertexInputs[i];
			newAttribute.binding = binding;
			newAttribute.format = ToVulkanInputFormat(vertexInput.inputFormat);
			newAttribute.offset = vertexInput.offset;
			newAttribute.location = vertexInput.location;
			inputDescriptions[i] = newAttribute;
		}
		bindingDescription.binding = binding;
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		bindingDescription.stride = stride;
	}
}