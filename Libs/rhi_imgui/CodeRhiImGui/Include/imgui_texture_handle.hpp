#pragma once
#include "imgui.h"
#include "Interface/texture.hpp"
#include "Interface/device.hpp"
#include <exception>
class VulkanImGuiTextureHandle;
class ImGuiTextureHandle {
public:
	virtual ~ImGuiTextureHandle() = default;
	ImTextureID textureId;
	virtual VulkanImGuiTextureHandle& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanImGuiTextureHandle"); }
	virtual rhi::Texture* GetTexture() = 0;
protected:
	
};