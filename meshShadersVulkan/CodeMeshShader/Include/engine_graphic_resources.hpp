#pragma once
#include "GLFW/glfw3.h"
#include "Interface/device.hpp"
#include "Interface/instance.hpp"
#include "Interface/surface.hpp"
#include "Interface/semaphore.hpp"
#include "Interface/fence.hpp"
#include "Interface/command_pool.hpp"
#include "service.hpp"
namespace msvk {
	class EngineGraphicResources : public Service {
	public :
		void Create(GLFWwindow* window);


		rhi::Device* GetDevice() { return device; }
		rhi::Swapchain* GetSwapchain() { return swapchain; }
		rhi::CommandPool* GetCommandPool() { return graphicCommandPool; }
		rhi::Instance* GetInstance() { return instance; }

		uint32_t GetFrameInFlightCount();
		uint32_t GetCurrentFrame();

		rhi::Semaphore* GetImageAvailableSemaphore(uint32_t index) { return imageAvailableSemaphores[index]; }
		rhi::Semaphore* GetImageRenderedSemaphore(uint32_t index) { return imageRenderedSemaphores[index]; }
		rhi::Fence* GetImageFence(uint32_t index) { return imageFences[index]; }
		rhi::Fence* GetCurrentFence() { return GetImageFence(GetCurrentFrame()); }

		~EngineGraphicResources();
	private :
		rhi::Instance* instance = nullptr;
		rhi::Device* device = nullptr;
		rhi::Surface* surface = nullptr;
		rhi::Swapchain* swapchain = nullptr;

		std::vector<rhi::Semaphore*> imageAvailableSemaphores;
		std::vector<rhi::Semaphore*> imageRenderedSemaphores;
		std::vector<rhi::Fence*> imageFences;

		rhi::CommandPool* graphicCommandPool;
	};
}