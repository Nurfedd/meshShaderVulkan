#include "engine_graphic_resources.hpp"
#include "Interface/graphic_api.hpp"
using namespace rhi;
namespace msvk {
	void EngineGraphicResources::Create(GLFWwindow* window) {
		instance = renderInterface->InitInstance();
		instance->Create("GoofyEngyne");

		surface = renderInterface->InitSurface();
		surface->Create(instance, window);
		PhysicalDevice* choosedPhysicalDevice = nullptr;
		std::vector<PhysicalDevice*> physicalDevices = instance->GetPhysicalDevices();

		std::vector<PhysicalDevice*> supportQueueDevices;

		for (uint32_t i = 0; i < physicalDevices.size(); i++) {
			PhysicalDevice* physicalDevice = physicalDevices[i];
			QueueSupportedFlags flags = physicalDevice->flags;
			if (flags & GRAPHICS_SUPPORT && flags & COMPUTE_SUPPORT && physicalDevice->SupportSurface(surface)) {
				supportQueueDevices.push_back(physicalDevice);
			}
		}

		for (uint32_t i = 0; i < supportQueueDevices.size(); i++) {
			PhysicalDevice* physicalDevice = physicalDevices[i];
			if (physicalDevice->isDiscreteGPU) {
				choosedPhysicalDevice = physicalDevice;
				break;
			}
		}
		if (!choosedPhysicalDevice) {
			throw std::exception("Couldn't find any GPU that for engine support.");
			return;
		}
		device = renderInterface->InitDevice();

		device->Create(instance, choosedPhysicalDevice, surface);
		surface->LoadFormats(device);
		// swapchain
		swapchain = renderInterface->InitSwapchain();

		SwapchainCreateInfo swapchainCreateInfo;
		swapchainCreateInfo.frameInFlight = 3;
		swapchainCreateInfo.presentMode = MAILBOX_MODE;
		swapchainCreateInfo.surface = surface;
		swapchainCreateInfo.surfaceFormat = SurfaceFormat(RGBA8_UNORM, SRGB_NON_LINEAR);

		swapchain->Create(device, swapchainCreateInfo);
		// semaphores

		uint32_t imageCount = swapchain->GetImageCount();

		imageAvailableSemaphores.resize(imageCount);
		imageRenderedSemaphores.resize(imageCount);
		imageFences.resize(imageCount);

		for (uint32_t i = 0; i < imageCount; i++) {
			Semaphore* imageAvailableSemaphore = renderInterface->InitSemaphore();
			Semaphore* imageRendererSemaphore = renderInterface->InitSemaphore();
			Fence* imageFence = renderInterface->InitFence();

			imageAvailableSemaphore->CreateBinary(device);
			imageRendererSemaphore->CreateBinary(device);
			imageFence->Create(device);

			imageAvailableSemaphores[i] = imageAvailableSemaphore;
			imageRenderedSemaphores[i] = imageRendererSemaphore;
			imageFences[i] = imageFence;
		}

		graphicCommandPool = renderInterface->InitCommandPool();
		graphicCommandPool->Create(device, device->GetQueue(GRAPHIC_QUEUE));
	}
	uint32_t EngineGraphicResources::GetFrameInFlightCount() {
		return swapchain->GetImageCount();
	}
	uint32_t EngineGraphicResources::GetCurrentFrame() {
		return swapchain->GetCurrentFrame();
	}
	EngineGraphicResources::~EngineGraphicResources() {
		uint32_t imageCount = GetFrameInFlightCount();

		for (uint32_t i = 0; i < imageCount; i++) {
			renderInterface->DestroySempahore(imageRenderedSemaphores[i], device);
			renderInterface->DestroySempahore(imageAvailableSemaphores[i], device);
			renderInterface->DestroyFence(imageFences[i], device);
		}
		renderInterface->DestroyCommandPool(graphicCommandPool, device);
		renderInterface->DestroySwapchain(swapchain, device);
		renderInterface->DestroyDevice(device);
		renderInterface->DestroySurface(surface, instance);
		renderInterface->DestroyInstance(instance);
	}
}