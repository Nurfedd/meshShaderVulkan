#include "editor.hpp"
#include "Graphics/SceneRenderers/triangle_renderer.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "service_locator.hpp"
#include "Graphics/engine_graphic_resources.hpp"
using namespace nino_engine;
namespace nino_editor {
	void NinoEditor::Run(int width, int height, const char* label) {
		Init(width, height, label);
		Loop();
		Destroy();
	}
	void NinoEditor::Init(int width, int height, const char* label) {
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		window = glfwCreateWindow(width, height, label, nullptr, nullptr);
		engine.Init(window);
		glfwMakeContextCurrent(window);

		engine.GetRendererController()->CreateNewRenderContext<TriangleRenderer>(nullptr, true);
		InitImGui();
	}
	void NinoEditor::InitImGui() {
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
		implCreateInfo.device = engineGraphicResources->Device;
		implCreateInfo.swapchain = engineGraphicResources->Swapchain;
		implCreateInfo.instance = engineGraphicResources->Instance;

		imGuiImplementation->Create(implCreateInfo);
		RhiSetCurrentImplementation(imGuiImplementation);

		RendererController* rendererController = engine.GetRendererController();
		rendererController->OnPrevBeginFrame.Add(this, &NinoEditor::OnRenderThreadPrevBeginFrame);
		rendererController->OnPostBeginFrame.Add(this, &NinoEditor::OnRenderThreadPrevRenderFrame);
		rendererController->OnCommandBufferRecorded.Add(this, &NinoEditor::RecordImGuiDrawData);
		rendererController->OnBeginDestroy.Add(this, &NinoEditor::OnRendererDestroy);
		//engine.GetRendererController()->AddDrawDataFunc(std::bind(&NinoEditor::RecordImGuiDrawData, this, std::placeholders::_1));
	}
	void NinoEditor::Loop() {
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
			engine.Update();
		}
	}
	void NinoEditor::OnRenderThreadPrevBeginFrame() {
		ImGui_ImplGlfw_NewFrame();
		imGuiImplementation->BeginFrame();
		ImGui::NewFrame();

		ImGui::Begin("oui");

		ImGui::Text(std::format("FPS : {}", uint32_t(ImGui::GetIO().Framerate)).c_str());
		ImGui::Text("ah oui");
		ImGui::End();
	}
	void NinoEditor::OnRenderThreadPrevRenderFrame() {
		ImGui::Render();
	}
	void NinoEditor::RecordImGuiDrawData(rhi::CommandBuffer* commandBuffer) {
		if (imGuiImplementation) {
			imGuiImplementation->AddDrawData(commandBuffer);
		}
	}

	void NinoEditor::OnRendererDestroy() {
		imGuiImplementation->Destroy();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyPlatformWindows();
		ImGui::DestroyContext();

	}
	void NinoEditor::Destroy() {
		engine.Destroy();
		glfwDestroyWindow(window);
		glfwTerminate();
	}
}