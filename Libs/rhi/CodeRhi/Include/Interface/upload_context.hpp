#pragma once
#include "device.hpp"

namespace rhi {
	class VulkanUploadContext;
	class UploadContext {
	public :
		virtual ~UploadContext() = default;
		virtual void Create(Device* device) = 0;
		virtual void Destroy(Device* device) = 0;
		virtual VulkanUploadContext& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanUploadContext"); }
	};
}