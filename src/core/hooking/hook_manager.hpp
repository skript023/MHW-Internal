#pragma once
#include "core/common.hpp"
#include "core/hooking/detour_hook.hpp"

namespace big
{
	// Low-level hooks owned by the framework itself. Game hooks are registered
	// by the game layer through hook_manager::add() so this layer never has to
	// know about a specific game.
	struct hooks
	{
		static void* convert_thread_to_fiber(void* param);

		static constexpr auto swapchain_num_funcs           = 19;
		static constexpr auto swapchain_present_index       = 8;
		static constexpr auto swapchain_draw_indexed_index  = 12;
		static constexpr auto swapchain_resizebuffers_index = 13;

		static HRESULT APIENTRY swapchain_present(IDXGISwapChain* this_, UINT sync_interval, UINT flags);
		static HRESULT APIENTRY swapchain_resizebuffers(IDXGISwapChain* this_, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swapchain_flags);
		static void APIENTRY swapchain_draw_indexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
		static void APIENTRY execute_command_lists(ID3D12CommandQueue* queue, UINT num_command_lists, ID3D12CommandList* const* pp_command_lists);

		static LRESULT wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static BOOL set_cursor_pos(int x, int y);
	};

	class hook_manager
	{
		friend struct hooks;

	public:
		explicit hook_manager();
		~hook_manager();

		void enable();
		void disable();
		void delete_disabled_hooks();

		// The game layer owns the pointer table, so it hands the framework the
		// addresses the framework has to hook.
		void install_swapchain_hooks(void** swapchain_methods, void* execute_command_lists_target);

		// Register any detour owned by a game or engine module. The callback must
		// be a function pointer with a matching signature and has to call
		// detour_base::get_original<callback>() to reach the original function.
		template<auto callback>
		static void add(std::string_view name, void* target)
		{
			detour_hook::add<callback>(name, target);
		}

	private:
		bool m_enabled{};
		WNDPROC m_og_wndproc{};
	};

	inline hook_manager* g_hook_manager{};
}
