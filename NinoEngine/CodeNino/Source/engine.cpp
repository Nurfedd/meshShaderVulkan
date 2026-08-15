#include "engine.hpp"
#include "Graphics/engine_graphic_resources.hpp"
#include "Interface/graphic_api.hpp"
#include "task_manager.hpp"
#include <iostream>
using namespace rhi;
using namespace mt;
namespace nino_engine {
	GLFWwindow* NinoEngine::Init(int width, int height, const char* windowTitle) {
		rhi::Init(VULKAN_API);
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		Window = glfwCreateWindow(width, height, windowTitle, nullptr, nullptr);

		EngineGraphicResources* engineResources = ServiceLocator::RegisterService<EngineGraphicResources>();
		TaskManager* taskManager = ServiceLocator::RegisterService<TaskManager>();

		engineResources->Create(Window);
		rendererController.Create();

		return Window;
	}

	void NinoEngine::Start() {
		while (!glfwWindowShouldClose(Window))
		{
			glfwPollEvents();
			Update();
		}
		Destroy();
	}
	void NinoEngine::Update() {
		// Build frame data and push to render thread queue
		int width, height;
		glfwGetWindowSize(Window, &width, &height);
		if (width > 0 && height > 0) {
			FrameData frameData{};
			rendererController.PushFrame(frameData);
		}
	}
	

	

	void NinoEngine::Destroy() {
		EngineGraphicResources* engineResources = ServiceLocator::Get<EngineGraphicResources>();
		Device* device = engineResources->Device;
		uint32_t frameCount = engineResources->GetFrameInFlightCount();

		device->WaitIdle();
		rendererController.Destroy();
		ServiceLocator::Clear();

		glfwDestroyWindow(Window);
		glfwTerminate();
	}
}