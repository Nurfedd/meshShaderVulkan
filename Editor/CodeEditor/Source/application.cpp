#include "application.hpp"
#include "SceneRenderers/triangle_renderer.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "service_locator.hpp"
#include "engine_graphic_resources.hpp"
using namespace msvk;
namespace msvk_editor {
	void Application::Run(int width, int height, const char* label) {
		Init(width, height, label);
		Loop();
		Destroy();
	}
	void Application::Init(int width, int height, const char* label) {
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		window = glfwCreateWindow(width, height, label, nullptr, nullptr);
		engine.Init(window);
		glfwMakeContextCurrent(window);

		engine.CreateNewRenderContext<TriangleRenderer>(nullptr, true);
		InitImGui();
	}
	void Application::InitImGui() {
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		/*io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/calibri.ttf", 19.0f);*/
		io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/trebuc.ttf", 20.0f);
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		
		ImGui_ImplGlfw_InitForVulkan(window, true);

		imGuiImplementation = RhiGetImGuiImplementation(VULKAN);
		
		BeginImpl implCreateInfo;
		
		EngineGraphicResources* engineGraphicResources = ServiceLocator::Get<EngineGraphicResources>();
		implCreateInfo.device = engineGraphicResources->GetDevice();
		implCreateInfo.swapchain = engineGraphicResources->GetSwapchain();
		implCreateInfo.instance = engineGraphicResources->GetInstance();

		imGuiImplementation->Create(implCreateInfo);
		RhiSetCurrentImplementation(imGuiImplementation);

		engine.AddDrawDataFunc(std::bind(&Application::RecordImGuiDrawData, this, std::placeholders::_1));
	}
	void Application::Loop() {
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();

			ImGui_ImplGlfw_NewFrame();
			imGuiImplementation->BeginFrame();
			ImGui::NewFrame();

			ImGui::Begin("oui");
			ImGui::Text("ah oui");
			ImGui::End();

			
			if (!engine.BeginFrame()) {
				continue;
			}

			ImGui::Render();

			engine.RenderFrame();
		}
	}

	void Application::RecordImGuiDrawData(rhi::CommandBuffer* commandBuffer) {
		if (imGuiImplementation) {
			imGuiImplementation->AddDrawData(commandBuffer);
		}
	}

	void Application::Destroy() {
		imGuiImplementation->Destroy();

		engine.Destroy();

		
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyPlatformWindows();
		ImGui::DestroyContext();

		glfwDestroyWindow(window);
		glfwTerminate();
	}
}