#pragma once
#include "imgui_texture_handle.hpp"
#include "IVulkan/vulkan_texture.hpp"
class VulkanImGuiTextureHandle : public ImGuiTextureHandle {
public :
	rhi::VulkanTexture vulkanTexture;
	VulkanImGuiTextureHandle& API_VULKAN() override { return *(this); }
	rhi::Texture* GetTexture() { return &vulkanTexture; }
};