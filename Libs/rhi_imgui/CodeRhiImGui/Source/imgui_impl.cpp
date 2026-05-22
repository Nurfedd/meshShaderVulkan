#include "imgui_impl.hpp"
#include "vulkan_imgui_impl.hpp"

Rhi_ImGui_Implementation* currentImplementation = nullptr;


Rhi_ImGui_Implementation* RhiGetImGuiImplementation(ImGuiImplentationApi implementationApi) {
	switch (implementationApi)
	{
	case VULKAN: return new Rhi_Vulkan_ImGui_Implementation;
	}
}

void RhiDestroyImGuiImplementation(Rhi_ImGui_Implementation* implementation) {
	implementation->Destroy();
	delete implementation;
}

Rhi_ImGui_Implementation* RhiGetCurrentImplementation() {
	return currentImplementation;
}
void RhiSetCurrentImplementation(Rhi_ImGui_Implementation* implementation) {
	currentImplementation = implementation;
}