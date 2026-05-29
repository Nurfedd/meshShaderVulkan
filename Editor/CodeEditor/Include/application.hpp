#pragma once
#include "GLFW/glfw3.h"
#include "engine.hpp"
#include "SceneRenderers/triangle_renderer.hpp"
#include "imgui_impl.hpp"
namespace msvk_editor {
	class Application {
	public :
		void Run(int width,int height,const char* label);
	private :
		void Init(int width, int height, const char* label);
		void RecordImGuiDrawData(rhi::CommandBuffer* commandBuffer);
		void InitImGui();
		void Loop();
		void Destroy();

		GLFWwindow* window = nullptr;
		msvk::Engine engine;

		Rhi_ImGui_Implementation* imGuiImplementation = nullptr;
	};
}