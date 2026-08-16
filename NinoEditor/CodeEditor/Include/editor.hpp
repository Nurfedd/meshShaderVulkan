#pragma once
#include "GLFW/glfw3.h"
#include "engine.hpp"
#include "Graphics/SceneRenderers/triangle_renderer.hpp"
#include "imgui_impl.hpp"
namespace nino_editor {
	class NinoEditor {
	public :
		void Run(int width,int height,const char* label);
	private :

		void Init(int width, int height, const char* label);
		void InitImGui();
		void OnRenderThreadPrevBeginFrame();
		void OnRenderThreadPrevRenderFrame();
		void RecordImGuiDrawData(rhi::CommandBuffer* commandBuffer);
		void OnRendererDestroy();
		void Destroy();

		GLFWwindow* window = nullptr;

		Rhi_ImGui_Implementation* imGuiImplementation = nullptr;
	};
}