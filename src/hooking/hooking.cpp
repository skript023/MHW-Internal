#include "common.hpp"
#include "function_types.hpp"
#include "gui.hpp"
#include "hooking.hpp"
#include "memory/module.hpp"
#include "pointers.hpp"
#include "renderer.hpp"

#include <MinHook.h>

namespace big
{
	hooking::hooking() : m_swapchain_hook(*g_pointers->m_swapchain, hooks::swapchain_num_funcs)
	{
		m_swapchain_hook.hook(hooks::swapchain_present_index, hooks::swapchain_present);
		m_swapchain_hook.hook(hooks::swapchain_resizebuffers_index, hooks::swapchain_resizebuffers);
		// detour_hook::add<hooks::swapchain_present>("SwapChainPresent", g_pointers->m_swapchain_methods[hooks::swapchain_present_index]),
		// detour_hook::add<hooks::swapchain_resizebuffers>("SwapChainResizeBuffers", g_pointers->m_swapchain_methods[hooks::swapchain_resizebuffers_index]),

		detour_hook::add<hooks::set_cursor_pos>("SetCursorPos", memory::module("user32.dll").get_export("SetCursorPos").as<void*>());
		detour_hook::add<hooks::convert_thread_to_fiber>("ConvertThreadToFiber", memory::module("kernel32.dll").get_export("ConvertThreadToFiber").as<void*>());
		detour_hook::add<hooks::material_handler>("Ignore Material", g_pointers->m_ignore_material);
		detour_hook::add<hooks::equipment_crafting>("Unlock Equipment", g_pointers->m_equipment_unlock);
		detour_hook::add<hooks::consumable>("Consumable", g_pointers->m_consumable);
		detour_hook::add<hooks::use_item>("Use Item", g_pointers->m_use_item);
		detour_hook::add<hooks::master_rank_experience>("Master Rank EXP Mult", g_pointers->m_master_rank_exp);
		detour_hook::add<hooks::high_rank_experience>("High Rank EXP Mult", g_pointers->m_highrank_exp);
		detour_hook::add<hooks::infinite_gathering>("Inf Gathering", g_pointers->m_inf_gathering);
		detour_hook::add<hooks::research_experience>("Research EXP Mult", g_pointers->m_research_exp);
		detour_hook::add<hooks::highlighted_armour>("Equipment Hook", g_pointers->m_armor_ptr);
		detour_hook::add<player_atk_stat>("Atk Hook", g_pointers->m_atk_ptr);
		//detour_hook::add<player_def_stat>(new detour_hook("Def Hook", g_pointers->m_def_ptr, &player_def_stat));
		detour_hook::add<player_affinity_stat>("Aff Hook", g_pointers->m_affinity_ptr);
		detour_hook::add<hooks::deploy_character>("Character Hook", g_pointers->m_character_ptr);
		detour_hook::add<hooks::highlighted_weapon>("Weapon Hook", g_pointers->m_weapon_ptr);
		detour_hook::add<hooks::meal_handler>("Meal Handler Hook", g_pointers->m_meal_handler);
		detour_hook::add<hooks::palico_experience_handler>("Meal Handler Hook", g_pointers->m_palico_experiece_handler);
		detour_hook::add<hooks::player_forward>("Player Coordinates Hook", g_pointers->m_player_forward);
		detour_hook::add<hooks::player_position>("Player Position Hook", g_pointers->m_player_position);
		detour_hook::add<hooks::waypoint>("Waypoint Position Hook", g_pointers->m_waypoint);
		detour_hook::add<hooks::monster_part_breaking>("Easy Part Break Hook", g_pointers->m_easy_breaker);
		detour_hook::add<hooks::monster_on_map>("Monster On Map Progress Hook", g_pointers->m_reveal_monster);
		detour_hook::add<hooks::bow_gun_effect>("Bow Gun Effect Hook", g_pointers->m_bow_gun_effect);

		g_hooking = this;
	}

	hooking::~hooking()
	{
		if (m_enabled)
			disable();

		g_hooking = nullptr;
	}

	void hooking::enable()
	{
		m_swapchain_hook.enable();

		m_og_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(g_pointers->m_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&hooks::wndproc)));
		base_hook::enable_all();

		MH_ApplyQueued();

		m_enabled = true;
	}

	void hooking::disable()
	{
		m_enabled = false;

		base_hook::enable_all();

		SetWindowLongPtr(g_pointers->m_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_og_wndproc));

		m_swapchain_hook.disable();

		MH_ApplyQueued();

		for (auto it : base_hook::hooks())
		{
			delete it;
		}
	}

	minhook_keepalive::minhook_keepalive()
	{
		MH_Initialize();
	}

	minhook_keepalive::~minhook_keepalive()
	{
		MH_Uninitialize();
	}

	void* hooks::convert_thread_to_fiber(void* param)
	{
		if (IsThreadAFiber())
		{
			return GetCurrentFiber();
		}

		return base_hook::get_original<convert_thread_to_fiber>()(param);
	}

	LRESULT APIENTRY hooks::wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (g_running)
		{
			g_renderer->wndproc(hwnd, msg, wparam, lparam);
		}

		return CallWindowProc(g_hooking->m_og_wndproc, hwnd, msg, wparam, lparam);
	}

	BOOL hooks::set_cursor_pos(int x, int y)
	{
		if (g_gui.m_opened)
			return true;

		return base_hook::get_original<set_cursor_pos>()(x, y);
	}
}
