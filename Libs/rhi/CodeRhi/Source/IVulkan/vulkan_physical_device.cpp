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
        int currentFlags = 0;
        for (VkQueueFamilyProperties& familyProperty : queueFamilies) {
            if (familyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                currentFlags = currentFlags | GRAPHICS_SUPPORT;

            if (familyProperty.queueFlags & VK_QUEUE_COMPUTE_BIT)
                currentFlags = currentFlags | COMPUTE_SUPPORT;
        }
        flags = static_cast<QueueSupportedFlags>(currentFlags);
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
    std::unordered_map<QueueType, uint32_t> VulkanPhysicalDevice::GetFamilyQueueIndices(Surface* surface) {
        std::unordered_map<QueueType, uint32_t> familyIndices;

        std::vector<uint32_t> queueCounts(queueFamilies.size());
        for (uint32_t i = 0; i < queueFamilies.size(); i++)
            queueCounts[i] = queueFamilies[i].queueCount;

        auto hasAvailableQueue = [&](uint32_t familyIndex) -> bool {
            return queueCounts[familyIndex] > 0;
            };

        for (uint32_t familyIndex = 0; familyIndex < queueFamilies.size(); familyIndex++)
        {
            const auto& family = queueFamilies[familyIndex];

            if ((family.queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
                !familyIndices.contains(GRAPHIC_QUEUE) &&
                hasAvailableQueue(familyIndex))
            {
                familyIndices[GRAPHIC_QUEUE] = familyIndex;
                queueCounts[familyIndex]--;
                break;
            }
        }

        for (uint32_t familyIndex = 0; familyIndex < queueFamilies.size(); familyIndex++)
        {
            const auto& family = queueFamilies[familyIndex];

            if ((family.queueFlags & VK_QUEUE_COMPUTE_BIT) &&
                !familyIndices.contains(COMPUTE_QUEUE) &&
                hasAvailableQueue(familyIndex))
            {
                familyIndices[COMPUTE_QUEUE] = familyIndex;
                queueCounts[familyIndex]--;
                break;
            }
        }

        if (surface)
        {
            VkSurfaceKHR vkSurface = surface->API_VULKAN().GetSurface();

            auto trySetPresent = [&](uint32_t familyIndex)
                {
                    VkBool32 presentSupport = false;
                    vkGetPhysicalDeviceSurfaceSupportKHR(device, familyIndex, vkSurface, &presentSupport);

                    if (presentSupport && hasAvailableQueue(familyIndex))
                    {
                        familyIndices[PRESENT_QUEUE] = familyIndex;
                        queueCounts[familyIndex]--;
                        return true;
                    }
                    return false;
                };

           
            if (familyIndices.contains(GRAPHIC_QUEUE))
            {
                uint32_t gfx = familyIndices[GRAPHIC_QUEUE];
                if (trySetPresent(gfx))
                    return familyIndices;
            }

           
            for (uint32_t familyIndex = 0; familyIndex < queueFamilies.size(); familyIndex++)
            {
                if (trySetPresent(familyIndex))
                    break;
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