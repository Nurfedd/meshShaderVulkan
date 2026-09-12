#include "IVulkan/vulkan_vertex_layout.hpp"
#include "IVulkan/enums/vulkan_input_format.hpp"
namespace rhi {
	void GetVulkanVertexLayoutObjects(const VertexLayout& vertexLayout, std::vector<VkVertexInputAttributeDescription>& inputDescriptions, VkVertexInputBindingDescription& bindingDescription) {
		inputDescriptions.resize(vertexLayout.inputCount);
		for (int i = 0; i < vertexLayout.inputCount; i++) {
			VkVertexInputAttributeDescription newAttribute{};
			VertexInput vertexInput = vertexLayout.vertexInputs[i];
			newAttribute.binding = vertexLayout.binding;
			newAttribute.format = ToVulkanInputFormat(vertexInput.inputFormat);
			newAttribute.offset = vertexInput.offset;
			newAttribute.location = vertexInput.location;
			inputDescriptions[i] = newAttribute;
		}
		bindingDescription.binding = vertexLayout.binding;
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		bindingDescription.stride = vertexLayout.stride;
	}
	
}