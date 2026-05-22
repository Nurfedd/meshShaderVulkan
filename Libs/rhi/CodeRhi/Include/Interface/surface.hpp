#pragma once
#include "Interface/instance.hpp"
#include "enums/format.hpp"
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <vector>

namespace rhi {
	class VulkanSurface;
	class Device;
	

	struct SurfaceFormat {
		SurfaceFormat() = default;
		SurfaceFormat(Format _format, ColorSpace _colorSpace) { format = _format; colorSpace = _colorSpace; }
		Format format;
		ColorSpace colorSpace;
	};

	class Surface {
	public:

		virtual ~Surface() = default;
		virtual void Create(Instance* instance, GLFWwindow* window) = 0;
		virtual void LoadFormats(Device* device) = 0;
		std::vector<PresentMode> GetPresentMode() {return presentModes;};
		std::vector<SurfaceFormat> GetSurfaceFormat() { return surfaceFormats; };

		bool SupportPresentMode(PresentMode presentMode);
		bool SupportSurfaceFormat(SurfaceFormat surfaceFormat);
		virtual void Destroy(Instance* instace) = 0;
		virtual VulkanSurface& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanSurface"); }
	protected :
		std::vector<PresentMode> presentModes;
		std::vector<SurfaceFormat> surfaceFormats;
		bool hasLoaded = false;
		
	};
}