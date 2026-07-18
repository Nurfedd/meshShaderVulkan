#include "IVulkan/vulkan_physical_device.hpp"
#include "IVulkan/vulkan_surface.hpp"
#include <string>
#include "Volk/volk.h"

namespace rhi {
	void VulkanPhysicalDevice::Load(VkPhysicalDevice physicalDevice) {
		device = physicalDevice;
		LoadInfo();
	}
	void VulkanPhysicalDevice::LoadInfo() {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);
        isDiscreteGPU = (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);


        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        queueFamilies.resize(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        for (VkQueueFamilyProperties& familyProperty : queueFamilies) {
            if (familyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                flags |= GRAPHICS_SUPPORT;

            if (familyProperty.queueFlags & VK_QUEUE_COMPUTE_BIT)
                flags |= COMPUTE_SUPPORT;
            if (familyProperty.queueFlags & VK_QUEUE_TRANSFER_BIT)
                flags |= TRANSFER_SUPPORT;
        }
	}
	bool VulkanPhysicalDevice::SupportExtensions() {
        uint32_t availableExtensionCount = 0;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &availableExtensionCount, nullptr);
        std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &availableExtensionCount, availableExtensions.data());
        bool supported = true;

        for (const std::string& required : deviceExtensions) {
            bool found = false;

            for (const VkExtensionProperties& available : availableExtensions) {
                if (required == available.extensionName) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                supported = false;
                break;
            }
        }
        return supported;
	}
	bool VulkanPhysicalDevice::SupportSurface(Surface* surface) {
        VulkanSurface& vulkanSurface = surface->API_VULKAN();
        VkSurfaceKHR vkSurface = vulkanSurface.GetSurface();

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        for (int familyIndex = 0; familyIndex < queueFamilyCount; familyIndex++) {
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, familyIndex, vkSurface, &presentSupport);
            if (presentSupport)
                return true;
        }
        return false;
	}
    std::unordered_map<QueueType, uint32_t> VulkanPhysicalDevice::GetFamilyQueueIndices(Surface* surface)
    {

        VulkanSurface& vulkanSurface = surface->API_VULKAN();

        std::unordered_map<QueueType, uint32_t> familyIndices;
        std::unordered_map<uint32_t, uint32_t> familyQueueCount;

        // first try to find a family that could support all family indices

       
        for (uint32_t familyIndex = 0; familyIndex < queueFamilies.size(); familyIndex++) {
            VkQueueFamilyProperties& queueProperty = queueFamilies[familyIndex];
            if (!familyIndices.contains(GRAPHIC_QUEUE)) {
                if (familyQueueCount[familyIndex] < queueProperty.queueCount && queueProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    familyIndices[GRAPHIC_QUEUE] = familyIndex;
                    familyQueueCount[familyIndex]++;
                }
            }
            if (!familyIndices.contains(COMPUTE_QUEUE)) {
                if (familyQueueCount[familyIndex] < queueProperty.queueCount && queueProperty.queueFlags & VK_QUEUE_COMPUTE_BIT) {
                    familyIndices[COMPUTE_QUEUE] = familyIndex;
                    familyQueueCount[familyIndex]++;
                }
            }
            if (!familyIndices.contains(TRANSFER_QUEUE)) {
                if (familyQueueCount[familyIndex] < queueProperty.queueCount && queueProperty.queueFlags & VK_QUEUE_TRANSFER_BIT) {
                    familyIndices[TRANSFER_QUEUE] = familyIndex;
                    familyQueueCount[familyIndex]++;
                }
            }
            if (!familyIndices.contains(PRESENT_QUEUE)) {
                if (familyQueueCount[familyIndex] < queueProperty.queueCount) {
                    VkBool32 presentSupport = false;
                    vkGetPhysicalDeviceSurfaceSupportKHR(device, familyIndex, vulkanSurface.GetSurface(), &presentSupport);
                    if (presentSupport) {
                        familyIndices[PRESENT_QUEUE] = familyIndex;
                        familyQueueCount[familyIndex]++;
                    }
                }
            }
        }
        
        return familyIndices;
    }
    VkPhysicalDeviceProperties VulkanPhysicalDevice::GetLimits() {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);
        return properties;
    }
}