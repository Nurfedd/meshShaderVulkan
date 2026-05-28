#pragma once
#include "GLFW/glfw3.h"
#include "engine.hpp"
#include "SceneRenderers/triangle_renderer.hpp"
namespace msvk {
	class Application {
	public :
		void Run(int width,int height,const char* label);
	private :
		void Init(int width, int height, const char* label);
		void Loop();
		void Destroy();

		GLFWwindow* window = nullptr;
		Engine engine;
	};
}