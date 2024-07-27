#pragma once
#include "common.hpp"
#include "detour_hook.hpp"
#include "vmt_hook.hpp"

#include "class/fwddec.hpp"

extern "C" void equipment();
extern "C" void player_atk_stat();
extern "C" void player_def_stat();
extern "C" void player_affinity_stat();

namespace big
{
	struct hooks
	{
		static void* convert_thread_to_fiber(void* param);

		static constexpr auto swapchain_num_funcs = 19;
		static constexpr auto swapchain_present_index = 8;
		static constexpr auto swapchain_draw_indexed_index = 12;
		static constexpr auto swapchain_resizebuffers_index = 13;
		static HRESULT APIENTRY swapchain_present(IDXGISwapChain* this_, UINT sync_interval, UINT flags);
		static HRESULT APIENTRY swapchain_resizebuffers(IDXGISwapChain* this_, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swapchain_flags);
		static void APIENTRY swapchain_draw_indexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);

		static LRESULT wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static BOOL set_cursor_pos(int x, int y);

		static int __fastcall material_handler(uint32_t a1, int a2);
		static bool __fastcall equipment_crafting(__int64 a1);
		static void __fastcall consumable(__int64 a1, int a2, DWORD* a3);
		static void __fastcall use_item(__int64 rcx, int r8l, int8_t a3);
		static __int64 __fastcall master_rank_experience(__int64 a1, uint32_t a2, char a3);
		static __int64 __fastcall high_rank_experience(__int64 a1, int a2);
		static __int64 __fastcall infinite_gathering(GatheringMonster* a1, __int64 a2);
		static void __fastcall research_experience(__int64 a1, uint32_t a2, float a3);
		static void __fastcall deploy_character(Character a1, uint32_t a2);
		static __int64 __fastcall highlighted_weapon(__int64 a1);
		static int __fastcall highlighted_armour(__int64 a1, __int64 a2, char a3, signed int* a4);
		static int __fastcall meal_handler(__int64 a1, MealStats* a2);
		static void __fastcall palico_experience_handler(__int64 a1, unsigned int a2, int a3, int a4);
		static int __fastcall player_coordinates(PlayerCoordinates* a1, __int64 a2);
		static int __fastcall player_position(PlayerPosition* a1);
		static int __fastcall waypoint(Waypoint* a1);
	};

	struct minhook_keepalive
	{
		minhook_keepalive();
		~minhook_keepalive();
	};

	class hooking
	{
		friend hooks;
	public:
		explicit hooking();
		~hooking();

		void enable();
		void disable();
	private:
		bool m_enabled{};
		minhook_keepalive m_minhook_keepalive;

		WNDPROC m_og_wndproc;

		detour_hook m_convert_thread_to_fiber_hook;
		detour_hook m_swapchain_present_hook;
		detour_hook m_swapchain_resizebuffers_hook;
		detour_hook m_swapchain_draw_indexed_hook;
		detour_hook m_set_cursor_pos_hook;

		detour_hook m_ignore_material_hook;
		detour_hook m_unlock_equipment_hook;
		detour_hook m_consumable_hook;
		detour_hook m_use_item_hook;
		detour_hook m_master_rank_mult_hook;
		detour_hook m_highrank_mult_hook;
		detour_hook m_inf_gathering_hook;
		detour_hook m_research_exp_hook;
		detour_hook m_equipment_hook;
		detour_hook m_player_atk_stat_hook;
		detour_hook m_player_def_stat_hook;
		detour_hook m_player_affinity_stat_hook;
		detour_hook m_character_ptr_hook;
		detour_hook m_weapon_hook;
		detour_hook m_meal_handler_hook;
		detour_hook m_palico_exp_handler_hook;
		detour_hook m_player_coordinates_hook;
		detour_hook m_player_position_hook;
		detour_hook m_waypoint_hook;
	};

	inline hooking* g_hooking{};
}
