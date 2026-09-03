#include "Graphics/engine_graphic_resources.hpp"
#include "Interface/graphic_api.hpp"
using namespace rhi;
namespace nino_engine {
	void EngineGraphicResources::Create(GLFWwindow* window) {
		Instance = renderInterface->InitInstance();
		Instance->Create("GoofyEngyne");

		Surface = renderInterface->InitSurface();
		Surface->Create(Instance, window);
		PhysicalDevice* choosedPhysicalDevice = nullptr;
		std::vector<PhysicalDevice*> physicalDevices = Instance->GetPhysicalDevices();

		std::vector<PhysicalDevice*> supportQueueDevices;

		for (uint32_t i = 0; i < physicalDevices.size(); i++) {
			PhysicalDevice* physicalDevice = physicalDevices[i];
			QueueSupportedFlags flags = physicalDevice->flags;
			if (flags & GRAPHICS_SUPPORT && flags & COMPUTE_SUPPORT && flags & TRANSFER_SUPPORT && physicalDevice->SupportSurface(Surface)) {
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
		Device = renderInterface->InitDevice();

		Device->Create(Instance, choosedPhysicalDevice, Surface);

		GraphicQueue = Device->GetGraphicQueue();
		PresentQueue = Device->GetPresentQueue();
		Surface->LoadFormats(Device);
		// swapchain
		Swapchain = renderInterface->InitSwapchain();

		SwapchainCreateInfo swapchainCreateInfo;
		swapchainCreateInfo.frameInFlight = 2;
		swapchainCreateInfo.presentMode = MAILBOX_MODE;
		swapchainCreateInfo.surface = Surface;
		swapchainCreateInfo.surfaceFormat = SurfaceFormat(RGBA8_UNORM, SRGB_NON_LINEAR);

		Swapchain->Create(Device, swapchainCreateInfo);
		// semaphores

		uint32_t imageCount = Swapchain->GetImageCount();

		imageAvailableSemaphores.resize(imageCount);
		imageRenderedSemaphores.resize(imageCount);
		imageFences.resize(imageCount);

		for (uint32_t i = 0; i < imageCount; i++) {
			Semaphore* imageAvailableSemaphore = renderInterface->InitSemaphore();
			Semaphore* imageRendererSemaphore = renderInterface->InitSemaphore();
			Fence* imageFence = renderInterface->InitFence();

			imageAvailableSemaphore->CreateBinary(Device);
			imageRendererSemaphore->CreateBinary(Device);
			imageFence->Create(Device);

			imageAvailableSemaphores[i] = imageAvailableSemaphore;
			imageRenderedSemaphores[i] = imageRendererSemaphore;
			imageFences[i] = imageFence;
		}

		GraphicCommandPool = renderInterface->InitCommandPool();
		GraphicCommandPool->Create(Device, Device->GetGraphicQueue());
	}
	uint32_t EngineGraphicResources::GetFrameInFlightCount() {
		return Swapchain->GetImageCount();
	}
	uint32_t EngineGraphicResources::GetCurrentSwapchainFrame() {
		return Swapchain->GetCurrentFrame();
	}
	
	// others service need all of these especially the device
	void EngineGraphicResources::OnPostDestroy() {
		uint32_t imageCount = GetFrameInFlightCount();

		for (uint32_t i = 0; i < imageCount; i++) {
			renderInterface->DestroySempahore(imageRenderedSemaphores[i], Device);
			renderInterface->DestroySempahore(imageAvailableSemaphores[i], Device);
			renderInterface->DestroyFence(imageFences[i], Device);
		}
		renderInterface->DestroyCommandPool(GraphicCommandPool, Device);
		renderInterface->DestroySwapchain(Swapchain, Device);
		renderInterface->DestroyDevice(Device);
		renderInterface->DestroySurface(Surface, Instance);
		renderInterface->DestroyInstance(Instance);
	}
}