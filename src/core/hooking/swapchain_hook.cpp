#include "core/hooking/hook_manager.hpp"
#include "core/ui/renderer.hpp"

namespace big
{
	HRESULT APIENTRY hooks::swapchain_present(IDXGISwapChain* this_, UINT sync_interval, UINT flags)
	{
		if (g_running)
		{
			if (!(flags & DXGI_PRESENT_TEST))
			{
				if (g_renderer && (g_renderer->m_init || g_renderer->init(this_)))
					g_renderer->on_present(this_);
			}
		}

		return base_hook::get_original<hooks::swapchain_present>()(this_, sync_interval, flags);
	}

	HRESULT APIENTRY hooks::swapchain_resizebuffers(IDXGISwapChain* this_, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swapchain_flags)
	{
		if (g_running && g_renderer)
		{
			g_renderer->pre_reset();

			auto result = base_hook::get_original<hooks::swapchain_resizebuffers>()(this_, buffer_count, width, height, new_format, swapchain_flags);

			if (SUCCEEDED(result))
			{
				g_renderer->post_reset(this_);
			}

			return result;
		}

		return base_hook::get_original<hooks::swapchain_resizebuffers>()(this_, buffer_count, width, height, new_format, swapchain_flags);
	}

	void APIENTRY hooks::swapchain_draw_indexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
	{
	}
}
