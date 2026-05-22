#pragma once
#include "imgui.h"
#include "imgui_texture_handle.hpp"
#include "Interface/render_interface.hpp"

enum ImGuiImplentationApi {
	VULKAN,
};

struct BeginImpl {
	rhi::Device* device;
	rhi::Instance* instance;
	rhi::Swapchain* swapchain;
	rhi::GraphicPipeline* renderingPipeline;
};



class Rhi_ImGui_Implementation {
public :
	virtual ~Rhi_ImGui_Implementation() = default;
	virtual void Create(BeginImpl beginImpl) = 0;
	virtual void BeginFrame() = 0;
	virtual ImGuiTextureHandle* CreateEmptyImageHandle(uint32_t width, uint32_t height) = 0;
	virtual void ResizeHandle(ImGuiTextureHandle* handle, uint32_t width, uint32_t height) = 0;
	virtual void DestroyHandle(ImGuiTextureHandle* handle) = 0;
	virtual void UploadPixel(ImGuiTextureHandle* handle, void* pixels, uint32_t channelCount) = 0;
	virtual void ApplyHandle(ImGuiTextureHandle* handle, ImVec2 size) = 0;
	virtual void AddDrawData(rhi::CommandBuffer* commandBuffer) = 0;
	virtual void Destroy() = 0;
};
extern Rhi_ImGui_Implementation* currentImplementation;

Rhi_ImGui_Implementation* RhiGetImGuiImplementation(ImGuiImplentationApi implementationApi);
void RhiDestroyImGuiImplementation(Rhi_ImGui_Implementation* implementation);

Rhi_ImGui_Implementation* RhiGetCurrentImplementation();
void RhiSetCurrentImplementation(Rhi_ImGui_Implementation* implementation);