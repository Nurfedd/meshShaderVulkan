#include "IVulkan/vulkan_graphic_pipeline.hpp"
#include "IVulkan/vulkan_vertex_layout.hpp"
#include "IVulkan/vulkan_shader.hpp"
#include "IVulkan/vulkan_set_layout.hpp"
#include "IVulkan/vulkan_push_constant_range.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/vulkan_command_buffer.hpp"
#include "IVulkan/enums/vulkan_rasterizer_state.hpp"
#include "IVulkan/enums/vulkan_input_format.hpp"
#include "IVulkan/enums/vulkan_format.hpp"
#include "Volk/volk.h"

namespace rhi {
	void VulkanGraphicPipeline::Create(Device* device, GraphicPipelineCreateInfo createInfo) {
		VulkanDevice& vulkanDevice = device->API_VULKAN();

		std::vector< VkPipelineShaderStageCreateInfo> shaderStages;
		CreateShaderStages(createInfo, shaderStages);

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = shaderStages.size();
		pipelineInfo.pStages = shaderStages.data();

		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(createInfo.vertexLayoutCount);
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions;

		
		for (int i = 0; i < createInfo.vertexLayoutCount; i++) {
			VulkanVertexLayout& vulkanVertexLayout = createInfo.vertexLayouts[i]->API_VULKAN();
			bindingDescriptions[i] = vulkanVertexLayout.GetBindingDescription();
			for (VkVertexInputAttributeDescription attribute : vulkanVertexLayout.GetInputDescriptions()) {
				attributeDescriptions.push_back(attribute);
			}
		}

		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = createInfo.vertexLayoutCount;
		vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
		vertexInputInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
		vertexInputInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.polygonMode = ToVulkanPolygonMode(createInfo.rasterizerInfo.polygonMode);
		rasterizer.cullMode = ToVulkanCullModeFlags(createInfo.rasterizerInfo.cullModeFlags);
		rasterizer.frontFace = ToVulkanFrontFace(createInfo.rasterizerInfo.frontFace);
		rasterizer.lineWidth = 1.0f;
		rasterizer.depthBiasEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		ColorAttachmentDescriptor* colorAttachments = createInfo.rasterizerInfo.colorAttachments;

		std::vector< VkPipelineColorBlendAttachmentState> blendsAttachments(createInfo.rasterizerInfo.attachmentCount);
		
		for (uint32_t i = 0; i < blendsAttachments.size(); i++) {
			VkPipelineColorBlendAttachmentState colorBlendAttachment{};
			colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			colorBlendAttachment.blendEnable = (colorAttachments[i].enableBlending) ? VK_TRUE : VK_FALSE;
			colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
			colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;

			colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;

			colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;

			colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
			blendsAttachments[i] = colorBlendAttachment;
		}
		
		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.attachmentCount = blendsAttachments.size();
		colorBlending.pAttachments = blendsAttachments.data();

		VkFormat depthFormat = ToVulkanFormat(createInfo.rasterizerInfo.depthFormat);

		VkPipelineDepthStencilStateCreateInfo depthStencil{};
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable = createInfo.rasterizerInfo.depthTestEnable;
		depthStencil.depthWriteEnable = createInfo.rasterizerInfo.depthWrite;
		depthStencil.depthCompareOp = ToVulkanCompareOp(createInfo.rasterizerInfo.depthOperator);
		depthStencil.depthBoundsTestEnable = VK_FALSE;
		depthStencil.stencilTestEnable = VK_FALSE;

		std::vector<VkDescriptorSetLayout> layouts(createInfo.setLayoutCount);

		for (int i = 0; i < createInfo.setLayoutCount; i++) {
			VulkanSetLayout& vulkanSetLayout = createInfo.setLayouts[i]->API_VULKAN();
			layouts[i] = vulkanSetLayout.GetLayout();
		}

		std::vector<VkPushConstantRange> pushConstantRanges(createInfo.pushConstantCount);
		for (int i = 0; i < createInfo.pushConstantCount; i++) {
			VulkanPushConstantRange& vulkanPushConstantRange = createInfo.pushConstants[i]->API_VULKAN();
			pushConstantRanges[i] = vulkanPushConstantRange.GetRange();
		}
  
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = createInfo.setLayoutCount;
		pipelineLayoutInfo.pSetLayouts = layouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = createInfo.pushConstantCount;
		pipelineLayoutInfo.pPushConstantRanges = pushConstantRanges.data();

		if (vkCreatePipelineLayout(vulkanDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!");
		}

		std::vector<VkFormat> vkFormats(createInfo.rasterizerInfo.attachmentCount);
		
		for (uint32_t i = 0; i < vkFormats.size(); i++) {
			vkFormats[i] = ToVulkanFormat(colorAttachments[i].format);
		}
		usedColorFormats = vkFormats;
		usedDepthFormat = depthFormat;
		

		VkPipelineRenderingCreateInfo renderingCI{};
		renderingCI.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
		renderingCI.colorAttachmentCount = vkFormats.size();
		renderingCI.pColorAttachmentFormats = vkFormats.data();
		renderingCI.depthAttachmentFormat = depthFormat;

		pipelineInfo.pNext = &renderingCI;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = pipelineLayout;
		pipelineInfo.renderPass = nullptr;
		pipelineInfo.subpass = 0;
		pipelineInfo.pDepthStencilState = &depthStencil;

		
		if (vkCreateGraphicsPipelines(vulkanDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
			throw std::runtime_error("failed to create graphics pipeline!");
		}
	}

	

	void VulkanGraphicPipeline::CreateShaderStages(GraphicPipelineCreateInfo createInfo, std::vector< VkPipelineShaderStageCreateInfo>& result) {
		if (createInfo.vertexShader) {
			VulkanShader& vulkanVertexShader = createInfo.vertexShader->API_VULKAN();
			VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
			vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			vertShaderStageInfo.module = vulkanVertexShader.GetModule();
			vertShaderStageInfo.pName = "main";
			result.push_back(vertShaderStageInfo);
		}
		if (createInfo.fragmentShader) {
			VulkanShader& vulkanFragmentShader = createInfo.fragmentShader->API_VULKAN();
			VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
			vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vertShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			vertShaderStageInfo.module = vulkanFragmentShader.GetModule();
			vertShaderStageInfo.pName = "main";
			result.push_back(vertShaderStageInfo);
		}
		if (createInfo.geometryShader) {
			VulkanShader& vulkanGeometryShader = createInfo.geometryShader->API_VULKAN();
			VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
			vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vertShaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
			vertShaderStageInfo.module = vulkanGeometryShader.GetModule();
			vertShaderStageInfo.pName = "main";
			result.push_back(vertShaderStageInfo);
		}
	}
	void VulkanGraphicPipeline::PushConstant(CommandBuffer* commandBuffer, uint32_t offset, size_t size, void* data, ShaderStagesFlags shaderStages) {
		PushConstantVk(commandBuffer->API_VULKAN(), offset, size, data, ToVulkanShaderStage(shaderStages));
	}
	void VulkanGraphicPipeline::BindDescriptorSet(CommandBuffer* commandBuffer, DescriptorSet** sets, uint32_t setCount, uint32_t firstSet) {
		std::vector<VulkanDescriptorSet> vulkanSets(setCount);
		for (uint32_t i = 0; i < setCount; i++) {
			vulkanSets[i] = sets[i]->API_VULKAN();
		}
		BindDescriptorSetsVk(commandBuffer->API_VULKAN(), vulkanSets.data(), setCount, firstSet);
	}
	void VulkanGraphicPipeline::Destroy(Device* device) {
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		VulkanPipeline::Destroy(vulkanDevice);
	}

	void VulkanGraphicPipeline::Bind(CommandBuffer* commandBuffer) {
		VulkanCommandBuffer& vulkanCommandBuffer = commandBuffer->API_VULKAN();
		VulkanPipeline::Bind(vulkanCommandBuffer);
	}
}