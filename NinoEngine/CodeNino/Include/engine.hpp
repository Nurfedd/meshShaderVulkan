#pragma once
#include "GLFW/glfw3.h"
#include "service_locator.hpp"
#include "Graphics/renderer_controller.hpp"
#include <functional>
#include <memory>

namespace nino_engine {

	class NinoEngine {
	public :
		GLFWwindow* Window;
		ServiceLocator EngineServices;
		GLFWwindow* Init(int width,int height,const char* windowTitle);
		void Start();
		
		RendererController* GetRendererController() { return &rendererController; }
		
		float GetFPS() { return 1.f / deltaTime; }
	private :
		

		float lastTime = 0.f;
		float deltaTime = 0.f;

		
		RendererController rendererController;

		void Update();
		void Destroy();
		
	};
	extern nino_engine::NinoEngine* GEngine;

	GLFWwindow* StartGlobalEngine(int width, int height, const char* windowTitle);
	void DeleteGlobalEngine();
}
