#include "IVulkan/vulkan_surface.hpp"
#include "IVulkan/vulkan_instance.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/vulkan_physical_device.hpp"
#include "IVulkan/enums/vulkan_format.hpp"
#include "Volk/volk.h"

namespace rhi {
	void VulkanSurface::Create(Instance* instance, GLFWwindow* window) {
		VulkanInstance& vulkanInstance = instance->API_VULKAN();
		glfwCreateWindowSurface(vulkanInstance.GetVkInstance(), window, nullptr, &surface);
	}
	void VulkanSurface::LoadFormats(Device* device) {
		LoadPresentMode(device);
		LoadSurfaceFormat(device);
	}

	void VulkanSurface::GetCapabilities(VkPhysicalDevice physicalDevice, VkSurfaceCapabilitiesKHR* capabilities) {
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, capabilities);
	}

	void VulkanSurface::LoadPresentMode(Device* device) {
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		VkPhysicalDevice physicalDevice = vulkanDevice.GetVkPhysicalDevice();

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
		presentModes.resize(presentModeCount);

		std::vector<VkPresentModeKHR> vkPresentModes(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, vkPresentModes.data());

		for (uint32_t i = 0; i < presentModeCount; i++) {
			presentModes[i] = ConvertVkPresent(vkPresentModes[i]);
		}
	}

	void VulkanSurface::LoadSurfaceFormat(Device* device) {
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		VkPhysicalDevice physicalDevice = vulkanDevice.GetVkPhysicalDevice();

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
		surfaceFormats.resize(formatCount);
		std::vector<VkSurfaceFormatKHR> vkFormats (formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, vkFormats.data());

		for (uint32_t i = 0; i < formatCount; i++) {
			SurfaceFormat newFormat;
			VkSurfaceFormatKHR vkSurfaceKHR = vkFormats[i];
			newFormat.colorSpace = FromVulkanColorSpace(vkSurfaceKHR.colorSpace);
			newFormat.format = FromVulkanFormat(vkSurfaceKHR.format);
			surfaceFormats[i] = newFormat;
		}
	}
	void VulkanSurface::Destroy(Instance* instance) {
		VulkanInstance vulkanInstance = instance->API_VULKAN();
		vkDestroySurfaceKHR(vulkanInstance.GetVkInstance(), surface, nullptr);
	}
}