#include "application.hpp"
#include "SceneRenderers/triangle_renderer.hpp"

namespace msvk {
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

		engine.CreateNewRenderContext<TriangleRenderer>(nullptr, true);
	}

	void Application::Loop() {
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
			engine.Update();
		}
	}

	void Application::Destroy() {
		engine.Destroy();
		glfwDestroyWindow(window);
		glfwTerminate();
	}
}