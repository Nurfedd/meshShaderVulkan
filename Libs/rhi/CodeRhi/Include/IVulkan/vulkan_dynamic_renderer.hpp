#include "Interface/dynamic_renderer.hpp"
#include "vulkan/vulkan.h"
namespace rhi {
	class VulkanDynamicRenderer : public DynamicRenderer {
	public:
		
		void Begin(CommandBuffer* commandBuffer, BeginRenderingInfo beginRenderingInfo) override;
		void End(CommandBuffer* commandBuffer) override;
		VulkanDynamicRenderer& API_VULKAN() override { return *(this); };
	private :
		
	};
}