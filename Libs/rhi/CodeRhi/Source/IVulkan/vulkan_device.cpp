#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/vulkan_surface.hpp"
#include "IVulkan/vulkan_instance.hpp"
#include "IVulkan/vulkan_device_queue.hpp"
#include "IVulkan/vulkan_fence.hpp"
#include "Volk/volk.h"
#include <algorithm>
#include <exception>

namespace rhi {
	void VulkanDevice::Create(Instance* instance,PhysicalDevice* _physicalDevice, Surface* surface) {
		
		Device::Create(instance,_physicalDevice, surface);
		bool shouldCreatePresentQueue = (surface != nullptr && physicalDevice->SupportSurface(surface));
		
		VulkanPhysicalDevice& vulkanPhysicalDevice = physicalDevice->API_VULKAN();
		VulkanInstance& vulkanInstance = instance->API_VULKAN();
		VkPhysicalDevice vkPhysicalDevice = GetVkPhysicalDevice();
		PhysicalDeviceQueueInfos physicalDeviceQueueInfos = vulkanPhysicalDevice.GetFamilyQueueIndices(surface);

		std::unordered_map<uint32_t, uint32_t> queueFamilyIndice = physicalDeviceQueueInfos.GetQueueFamilyIndiceAndCount();

		if (!(physicalDeviceQueueInfos.graphicQueue.IsValid() && physicalDeviceQueueInfos.computeQueue.IsValid())) {
			throw std::exception("Physical device doesnt support enough features.");
		}
		
		if (!(vulkanPhysicalDevice.FamilySupportPresent(physicalDeviceQueueInfos.graphicQueue.familyIndice, surface->API_VULKAN()))) {
			throw std::exception("Graphic family doesn't support present");
		}

		std::vector< VkDeviceQueueCreateInfo> queuesCreateInfos (queueFamilyIndice.size());
		std::vector<std::vector<float>> queuePriorities(queueFamilyIndice.size());
		int i = 0;

		

		for (auto& pair : queueFamilyIndice) {
			queuePriorities[i].resize(pair.second);
			std::fill(queuePriorities[i].begin(), queuePriorities[i].end(), 1.f);
			VkDeviceQueueCreateInfo queueInfo{};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.queueCount = pair.second;
			queueInfo.queueFamilyIndex = pair.first;
			queueInfo.pQueuePriorities = queuePriorities[i].data();
			queuesCreateInfos[i] = queueInfo;
			i++;
		}

		VkPhysicalDeviceDynamicRenderingFeatures dynamicRendering{};
		dynamicRendering.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES;
		dynamicRendering.dynamicRendering = VK_TRUE;
		dynamicRendering.pNext = nullptr;

		VkPhysicalDeviceFeatures2 features2{};
		features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		features2.features.samplerAnisotropy = VK_TRUE;
		features2.features.multiDrawIndirect = VK_TRUE;
		features2.features.drawIndirectFirstInstance = VK_TRUE;
		

		VkPhysicalDeviceVulkan12Features vulkan12Features{};
		vulkan12Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
		vulkan12Features.bufferDeviceAddress = VK_TRUE;
		vulkan12Features.descriptorIndexing = VK_TRUE;
		vulkan12Features.timelineSemaphore = VK_TRUE;
		
		vulkan12Features.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
		vulkan12Features.runtimeDescriptorArray = VK_TRUE;
		vulkan12Features.descriptorBindingVariableDescriptorCount = VK_TRUE;
		vulkan12Features.descriptorBindingPartiallyBound = VK_TRUE;
		vulkan12Features.descriptorBindingSampledImageUpdateAfterBind = VK_TRUE;
		vulkan12Features.descriptorBindingStorageBufferUpdateAfterBind = VK_TRUE;
		vulkan12Features.descriptorBindingUniformBufferUpdateAfterBind = VK_TRUE;
		

		VkPhysicalDeviceSynchronization2Features synchronizationFeatures{};
		synchronizationFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES;
		synchronizationFeatures.synchronization2 = VK_TRUE;

		VkPhysicalDeviceMeshShaderFeaturesEXT meshShaderFeatures{};
		meshShaderFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT;
		meshShaderFeatures.meshShader = VK_TRUE;
		meshShaderFeatures.taskShader = VK_TRUE;
		meshShaderFeatures.primitiveFragmentShadingRateMeshShader = VK_FALSE;
		meshShaderFeatures.multiviewMeshShader = VK_FALSE;
		meshShaderFeatures.meshShaderQueries = VK_FALSE;
		

		features2.pNext = &dynamicRendering;
		dynamicRendering.pNext = &vulkan12Features;
		vulkan12Features.pNext = &synchronizationFeatures;
		synchronizationFeatures.pNext = &meshShaderFeatures;
		meshShaderFeatures.pNext = nullptr;
		
		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = queuesCreateInfos.data();
		createInfo.queueCreateInfoCount = queuesCreateInfos.size();
		createInfo.pEnabledFeatures = nullptr;
		createInfo.enabledExtensionCount = deviceExtensions.size();
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = &features2;

		
		if (vkCreateDevice(vkPhysicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
			throw std::exception("failed to create logical Device!");
		}
		volkLoadDevice(device);
		
		GraphicQueue = std::make_shared<VulkanDeviceQueue>(device,physicalDeviceQueueInfos.graphicQueue.familyIndice);
		PresentQueue = GraphicQueue;

		ComputeQueue = std::make_unique<VulkanDeviceQueue>(device, physicalDeviceQueueInfos.computeQueue.familyIndice);

		if (physicalDeviceQueueInfos.transferQueueCpuToGpu.IsValid()) {
			PrimaryTransferQueue = std::make_unique<VulkanDeviceQueue>(device, physicalDeviceQueueInfos.transferQueueCpuToGpu.familyIndice);
		}
		if (physicalDeviceQueueInfos.transferQueueGpuToCpu.IsValid()) {
			SecondaryTransferQueue = std::make_unique<VulkanDeviceQueue>(device, physicalDeviceQueueInfos.transferQueueGpuToCpu.familyIndice);
		}
		InitAllocator(vulkanInstance.GetVkInstance());
	}
	void VulkanDevice::WaitForFencesVk(VulkanFence* fences, uint32_t count, bool waitForAll, uint64_t timeOut) {
		VkBool32 waitAll = (waitForAll) ? VK_TRUE : VK_FALSE;
		std::vector<VkFence> vkFences(count);
		for (uint32_t i = 0; i < count; i++) {
			vkFences[i] = fences[i].GetFence();
		}
		vkWaitForFences(device, count, vkFences.data(), waitAll, timeOut);
	}
	void VulkanDevice::WaitForFences(Fence** fences, uint32_t count,bool waitForAll, uint64_t timeOut) {
		std::vector<VulkanFence> vkFences(count);
		for (uint32_t i = 0; i < count; i++) {
			vkFences[i] = fences[i]->API_VULKAN();
		}
		WaitForFencesVk(vkFences.data(), count, waitForAll, timeOut);
	}
	void VulkanDevice::ResetFencesVk(VulkanFence* fences, uint32_t count) {
		std::vector<VkFence> vkFences(count);
		for (uint32_t i = 0; i < count; i++) {
			vkFences[i] = fences[i].GetFence();
		}
		vkResetFences(device, count, vkFences.data());
	}
	void VulkanDevice::ResetFences(Fence** fences, uint32_t count) {
		std::vector<VulkanFence> vkFences(count);
		
		for (uint32_t i = 0; i < count; i++) {
			vkFences[i] = fences[i]->API_VULKAN();
		}
		ResetFencesVk(vkFences.data(), count);
	}
	
	void VulkanDevice::InitAllocator(VkInstance instance) {
		VmaVulkanFunctions vkFunctions{};
		vkFunctions.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
		vkFunctions.vkGetDeviceProcAddr = vkGetDeviceProcAddr;
		vkFunctions.vkCreateImage = vkCreateImage;
		vkFunctions.vkCreateBuffer = vkCreateBuffer;

		VmaAllocatorCreateInfo allocatorCI{};
		allocatorCI.physicalDevice = GetVkPhysicalDevice();
		allocatorCI.device = device;
		allocatorCI.pVulkanFunctions = &vkFunctions;
		allocatorCI.vkInstance = instance;
		allocatorCI.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT; // buffer device adress
		if (vmaCreateAllocator(&allocatorCI, &allocator) != VK_SUCCESS) {
			throw std::exception("canno't create VmaAllocator");
		}
	}

	void VulkanDevice::WaitIdle() {
		vkDeviceWaitIdle(device);
	}
	void VulkanDevice::Destroy() {
		Device::Destroy();
		vmaDestroyAllocator(allocator);
		vkDestroyDevice(device, nullptr);
	}
}
