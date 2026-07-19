#pragma once
#include "GLFW/glfw3.h"
#include "Graphics/renderer_controller.hpp"
#include "service_locator.hpp"
#include <functional>
#include <memory>

namespace nino_engine {
	
	class NinoEngine {
	public :
		void Init(GLFWwindow* window);
		
		void Update();
		void Destroy();

		RendererController* GetRendererController() { return &rendererController; }
		
		float GetFPS() { return 1.f / deltaTime; }
	private :
		

		float lastTime = 0.f;
		float deltaTime = 0.f;
		
		RendererController rendererController;
	};
}