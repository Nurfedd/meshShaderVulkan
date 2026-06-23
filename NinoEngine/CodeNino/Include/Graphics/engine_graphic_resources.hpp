#pragma once
#include "GLFW/glfw3.h"
#include "Interface/device.hpp"
#include "Interface/instance.hpp"
#include "Interface/surface.hpp"
#include "Interface/semaphore.hpp"
#include "Interface/fence.hpp"
#include "Interface/command_pool.hpp"
#include "service.hpp"
namespace nino_engine {
	class EngineGraphicResources : public Service {
	public :

		~EngineGraphicResources() override;

		void Create(GLFWwindow* window);

		uint32_t GetFrameInFlightCount();
		uint32_t GetCurrentFrame();

		rhi::Semaphore* GetImageAvailableSemaphore(uint32_t index) { return imageAvailableSemaphores[index]; }
		rhi::Semaphore* GetImageRenderedSemaphore(uint32_t index) { return imageRenderedSemaphores[index]; }
		rhi::Fence* GetImageFence(uint32_t index) { return imageFences[index]; }
		rhi::Fence* GetCurrentFence() { return GetImageFence(GetCurrentFrame()); }

		rhi::Instance* Instance = nullptr;
		rhi::Device* Device = nullptr;
		rhi::DeviceQueue* GraphicQueue = nullptr;
		rhi::DeviceQueue* PresentQueue = nullptr;
		rhi::Surface* Surface = nullptr;
		rhi::Swapchain* Swapchain = nullptr;

		rhi::CommandPool* GraphicCommandPool = nullptr;



	private :

		std::vector<rhi::Semaphore*> imageAvailableSemaphores;
		std::vector<rhi::Semaphore*> imageRenderedSemaphores;
		std::vector<rhi::Fence*> imageFences;

		
	};
}