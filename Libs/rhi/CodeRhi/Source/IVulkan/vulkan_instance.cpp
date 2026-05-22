#include "IVulkan/vulkan_instance.hpp"
#include "IVulkan/vulkan_validation_layers.hpp"
#include "IVulkan/vulkan_physical_device.hpp"
#include "Volk/volk.h"
#include "GLFW/glfw3.h"
#include <exception>
#include <iostream>

namespace rhi {
	void VulkanInstance::Create(const char* applicationName) {
		if (enableValidationLayers && !CheckValidationLayers()) {
			throw std::exception("validation layers requested, but not available!");
		}
		VkApplicationInfo appInfo;
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pNext = nullptr;
		appInfo.pApplicationName = applicationName;
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = NULL;
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_4;

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledLayerCount = 0;
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;

		VkValidationFeatureEnableEXT enables[] = {
			VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT,
			VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT
		};

		VkValidationFeaturesEXT features{};
		features.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
		features.enabledValidationFeatureCount = 2;
		features.pEnabledValidationFeatures = enables;

		createInfo.pNext = &features;

		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateInstance(&createInfo, nullptr, &vkInstance) != VK_SUCCESS) {
			throw std::exception("failed to create instance!");
		}
		volkLoadInstance(vkInstance);
	}

	void VulkanInstance::LoadPhysicalDevices() {
		uint32_t count = 0;
		vkEnumeratePhysicalDevices(vkInstance, &count, nullptr);

		std::vector<VkPhysicalDevice> vkDevices(count);
		vkEnumeratePhysicalDevices(vkInstance, &count, vkDevices.data());

		for (VkPhysicalDevice vkDev : vkDevices)
		{
			VulkanPhysicalDevice* dev = new VulkanPhysicalDevice;
			dev->Load(vkDev);
			if (dev->SupportExtensions())
			{
				physicalDeviceViews.push_back(dev);
			}
 		}
	}
	void VulkanInstance::Destroy() {
		if (vkInstance)
			vkDestroyInstance(vkInstance, nullptr);
		for (int i = 0; i < physicalDeviceViews.size(); i++) {
			delete physicalDeviceViews[i];
		}
	}
}