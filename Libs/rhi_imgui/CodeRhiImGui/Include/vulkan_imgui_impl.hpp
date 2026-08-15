#include "imgui_impl.hpp"
#include "IVulkan/vulkan_descriptor_pool.hpp"
#include "IVulkan/vulkan_sampler.hpp"
#include "IVulkan/vulkan_texture.hpp"
#include "imgui_texture_handle.hpp"
#include <unordered_map>
class Rhi_Vulkan_ImGui_Implementation : public Rhi_ImGui_Implementation {
public :
	void Create(BeginImpl beginImpl) override;
	void AddDrawData(rhi::CommandBuffer* commandBuffer) override;
	
	ImGuiTextureHandle* CreateEmptyImageHandle(uint32_t width, uint32_t height) override;
	void ResizeHandle(ImGuiTextureHandle* handle, uint32_t width, uint32_t height) override;
	void DestroyHandle(ImGuiTextureHandle* handle) override;
	void UploadPixel(ImGuiTextureHandle* handle,void* pixels,uint32_t channelCount) override;
	void ApplyHandle(ImGuiTextureHandle* handle, ImVec2 size) override;
	void BeginFrame() override;
	void Destroy() override;
private :
	ImTextureID CreateId(rhi::VulkanTexture& vulkanTexture);
	void DestroyTexture(ImTextureID textureId);
	rhi::Device* device;
	rhi::VulkanDescriptorPool vulkanDescriptorPool;
	rhi::VulkanSampler vulkanSampler;
	rhi::VulkanUploadContext vulkanUploadContext;
};