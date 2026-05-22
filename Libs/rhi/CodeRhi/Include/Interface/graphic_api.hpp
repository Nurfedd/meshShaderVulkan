#pragma once
#include <functional>
#include "Interface/render_interface.hpp"
namespace rhi {
	enum RENDER_API
	{
		VULKAN_API,
	};
	extern bool hasInit;
	extern RENDER_API renderApi;
	extern RenderInterface* renderInterface;

	void Init(RENDER_API renderAPI);
	void Switch(RENDER_API newRenderAPI);
	void Close();
	RENDER_API GetRenderAPI();
}