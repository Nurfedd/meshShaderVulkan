#include "vulkan_imgui_impl.hpp"
#include "imgui_impl_vulkan.h"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/vulkan_device_queue.hpp"
#include "IVulkan/vulkan_descriptor_pool.hpp"
#include "IVulkan/vulkan_graphic_pipeline.hpp"
#include "IVulkan/vulkan_instance.hpp"
#include "IVulkan/vulkan_command_buffer.hpp"
#include "IVulkan/vulkan_texture.hpp"
#include "IVulkan/vulkan_sampler.hpp"
#include "vulkan_imgui_texture_handle.hpp"

using namespace rhi;

void Rhi_Vulkan_ImGui_Implementation::Create(BeginImpl beginInfo) {
	Device* device = beginInfo.device;
	uint32_t imageCount = beginInfo.swapchain->GetImageCount();

	
	this->device = beginInfo.device;
	VulkanDevice& vulkanDevice = device->API_VULKAN();
	DeviceQueue* graphicQueue = vulkanDevice.GetGraphicQueue();
	VulkanDeviceQueue vulkanGraphicQueue = graphicQueue->API_VULKAN();
	VulkanInstance& vulkanInstance = beginInfo.instance->API_VULKAN();

	VkFormat colorFormat = VK_FORMAT_R8G8B8A8_UNORM;

	vulkanDescriptorPool.CreatePoolForImGui(vulkanDevice);

	ImGui_ImplVulkan_InitInfo initInfo{};

	initInfo.ApiVersion = vulkanInstance.GetApiVersion();
	initInfo.Instance = vulkanInstance.GetVkInstance();
	initInfo.UseDynamicRendering = true;
	initInfo.Allocator = nullptr;
	initInfo.PhysicalDevice = vulkanDevice.GetVkPhysicalDevice();
	initInfo.Device = vulkanDevice;

	initInfo.ImageCount = imageCount;
	initInfo.MinImageCount = imageCount;
	initInfo.Queue = vulkanGraphicQueue.GetVkQueue();
	initInfo.QueueFamily = vulkanGraphicQueue.GetFamilyIndex();
	initInfo.DescriptorPool = vulkanDescriptorPool.GetPool();

	ImGui_ImplVulkan_PipelineInfo pipelineInfo{};

	VkPipelineRenderingCreateInfoKHR renderingInfo{};
	renderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
	renderingInfo.pColorAttachmentFormats = &colorFormat;
	renderingInfo.colorAttachmentCount = 1;
	renderingInfo.depthAttachmentFormat = VK_FORMAT_UNDEFINED;

	pipelineInfo.PipelineRenderingCreateInfo = renderingInfo;

	initInfo.PipelineInfoMain = pipelineInfo;

	ImGui_ImplVulkan_Init(&initInfo);
	SamplerCreateInfoVulkan samplerInfo;

	samplerInfo.useMipmap = false;
	samplerInfo.useAnisotropy = false;
	samplerInfo.compareEnable = false;

	samplerInfo.filter = VK_FILTER_LINEAR;
	samplerInfo.adressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.adressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.adressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	vulkanSampler.CreateVk(vulkanDevice, samplerInfo);
	vulkanUploadContext.Create(device);
}

void Rhi_Vulkan_ImGui_Implementation::AddDrawData(CommandBuffer* commandBuffer) {
	VulkanCommandBuffer& vulkanCommandBuffer = commandBuffer->API_VULKAN();
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), vulkanCommandBuffer.commandBuffer, nullptr);
}

ImGuiTextureHandle* Rhi_Vulkan_ImGui_Implementation::CreateEmptyImageHandle(uint32_t width, uint32_t height) {
	VulkanDevice& vulkanDevice = device->API_VULKAN();
	VulkanTexture newTexture;
	VulkanTextureCreateInfo createInfo;
	createInfo.height = height;
	createInfo.width = width;
	createInfo.mipCount = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	createInfo.textureFormat = VK_FORMAT_R8G8B8A8_UNORM;
	newTexture.CreateVk(vulkanDevice, createInfo);

	VulkanImGuiTextureHandle* newHandle = new VulkanImGuiTextureHandle;
	newHandle->textureId = CreateId(newTexture);
	newHandle->vulkanTexture = newTexture;
	return newHandle;
}

void Rhi_Vulkan_ImGui_Implementation::ResizeHandle(ImGuiTextureHandle* handle, uint32_t width, uint32_t height) {
	VulkanDevice& vulkanDevice = device->API_VULKAN();
	VulkanImGuiTextureHandle& vulkanTextureHandle = handle->API_VULKAN();
	VulkanTexture& vulkanTexture = vulkanTextureHandle.vulkanTexture;

	if (vulkanTexture.GetHeight() == height && vulkanTexture.GetWidth() == width) // no need to resize
		return;
	
	vulkanDevice.WaitIdle();
	vulkanTexture.ResizeVk(vulkanDevice, width, height);
	DestroyTexture(vulkanTextureHandle.textureId);
	vulkanTextureHandle.textureId = CreateId(vulkanTexture);
}

void Rhi_Vulkan_ImGui_Implementation::DestroyHandle(ImGuiTextureHandle* handle) {
	VulkanDevice& vulkanDevice = device->API_VULKAN();
	vulkanDevice.WaitIdle();
	VulkanImGuiTextureHandle& vulkanHandle = handle->API_VULKAN();

	DestroyTexture(vulkanHandle.textureId);
	vulkanHandle.vulkanTexture.DestroyVk(vulkanDevice);
	delete handle;
}

void Rhi_Vulkan_ImGui_Implementation::UploadPixel(ImGuiTextureHandle* handle, void* pixels, uint32_t channelCount) {
	VulkanDevice& vulkanDevice = device->API_VULKAN();
	VulkanImGuiTextureHandle& vulkanHandle = handle->API_VULKAN();
	VulkanTexture& vulkanTexture = vulkanHandle.vulkanTexture;
	
	vulkanTexture.UploadVk(vulkanDevice, vulkanUploadContext, pixels, channelCount);
}

void Rhi_Vulkan_ImGui_Implementation::ApplyHandle(ImGuiTextureHandle* handle, ImVec2 size) {
	ImGui::Image(handle->textureId, size);
}

ImTextureID Rhi_Vulkan_ImGui_Implementation::CreateId(VulkanTexture& vulkanTexture) {
	VkDescriptorSet set = ImGui_ImplVulkan_AddTexture(
		vulkanSampler.GetSampler(),
		vulkanTexture.GetImageView()->GetVkImageView(),
		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	ImTextureID textureId = (ImTextureID)set;
	return textureId;
}



void Rhi_Vulkan_ImGui_Implementation::DestroyTexture(ImTextureID textureId) {
	VkDescriptorSet set = (VkDescriptorSet)textureId;
	ImGui_ImplVulkan_RemoveTexture(set);
}

void Rhi_Vulkan_ImGui_Implementation::BeginFrame() {
	ImGui_ImplVulkan_NewFrame();
}

void Rhi_Vulkan_ImGui_Implementation::Destroy() {
	VulkanDevice& vulkanDevice = device->API_VULKAN();
	vulkanDevice.WaitIdle();
	ImGui_ImplVulkan_Shutdown();
	vulkanDescriptorPool.DestroyVk(vulkanDevice);
	vulkanSampler.DestroyVk(vulkanDevice);
	vulkanUploadContext.DestroyVk(vulkanDevice);
}