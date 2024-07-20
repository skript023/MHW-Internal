#include "common.hpp"
#include "function_types.hpp"
#include "logger.hpp"
#include "gui.hpp"
#include "hooking.hpp"
#include "memory/module.hpp"
#include "pointers.hpp"
#include "renderer.hpp"

#include <MinHook.h>

namespace big
{
	hooking::hooking() :
		m_swapchain_present_hook("SwapChainPresent", g_pointers->m_swapchain_methods[hooks::swapchain_present_index], &hooks::swapchain_present),
		m_swapchain_resizebuffers_hook("SwapChainResizeBuffers", g_pointers->m_swapchain_methods[hooks::swapchain_resizebuffers_index], &hooks::swapchain_resizebuffers),
		m_swapchain_draw_indexed_hook("SwapChainDrawIndexed", g_pointers->m_swapchain_methods[hooks::swapchain_draw_indexed_index], &hooks::swapchain_draw_indexed),
		m_set_cursor_pos_hook("SetCursorPos", memory::module("user32.dll").get_export("SetCursorPos").as<void*>(), &hooks::set_cursor_pos),
		m_convert_thread_to_fiber_hook("ConvertThreadToFiber", memory::module("kernel32.dll").get_export("ConvertThreadToFiber").as<void*>(), &hooks::convert_thread_to_fiber),
		m_ignore_material_hook("Ignore Material", g_pointers->m_ignore_material, &hooks::material_handler),
		m_unlock_equipment_hook("Unlock Equipment", g_pointers->m_equipment_unlock, &hooks::equipment_crafting),
		m_consumable_hook("Consumable", g_pointers->m_consumable, &hooks::consumable),
		m_use_item_hook("Use Item", g_pointers->m_use_item, &hooks::use_item),
		m_master_rank_mult_hook("Master Rank EXP Mult", g_pointers->m_master_rank_exp, &hooks::master_rank_experience),
		m_highrank_mult_hook("High Rank EXP Mult", g_pointers->m_highrank_exp, &hooks::high_rank_experience),
		m_inf_gathering_hook("High Rank EXP Mult", g_pointers->m_inf_gathering, &hooks::infinite_gathering),
		m_research_exp_hook("Research EXP Mult", g_pointers->m_research_exp, &hooks::research_experience),
		m_equipment_hook("Equipment Hook", g_pointers->m_armor_ptr, &hooks::highlighted_armour),
		m_player_atk_stat_hook("Atk Hook", g_pointers->m_atk_ptr, &player_atk_stat),
		m_player_def_stat_hook("Def Hook", g_pointers->m_def_ptr, &player_def_stat),
		m_player_affinity_stat_hook("Aff Hook", g_pointers->m_affinity_ptr, &player_affinity_stat),
		m_character_ptr_hook("Character Hook", g_pointers->m_character_ptr, &hooks::deploy_character),
		m_weapon_hook("Weapon Hook", g_pointers->m_weapon_ptr, &hooks::highlighted_weapon),
		m_meal_handler_hook("Meal Handler Hook", g_pointers->m_meal_handler, &hooks::meal_handler)
	{
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
		m_character_ptr_hook.enable();
		m_swapchain_present_hook.enable();
		m_swapchain_resizebuffers_hook.enable();
		m_swapchain_draw_indexed_hook.enable();
		m_og_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(g_pointers->m_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&hooks::wndproc)));
		m_set_cursor_pos_hook.enable();
		m_convert_thread_to_fiber_hook.enable();

		m_ignore_material_hook.enable();
		m_unlock_equipment_hook.enable();
		m_consumable_hook.enable();
		m_use_item_hook.enable();
		m_highrank_mult_hook.enable();
		m_master_rank_mult_hook.enable();
		m_inf_gathering_hook.enable();
		m_research_exp_hook.enable();
		m_equipment_hook.enable();
		m_player_atk_stat_hook.enable();
		m_player_def_stat_hook.enable();
		m_player_affinity_stat_hook.enable();
		m_weapon_hook.enable();
		m_meal_handler_hook.enable();

		m_enabled = true;
	}

	void hooking::disable()
	{
		m_enabled = false;

		m_meal_handler_hook.disable();
		m_weapon_hook.disable();
		m_player_affinity_stat_hook.disable();
		m_player_def_stat_hook.disable();
		m_player_atk_stat_hook.disable();
		m_equipment_hook.disable();
		m_research_exp_hook.disable();
		m_inf_gathering_hook.disable();
		m_master_rank_mult_hook.disable();
		m_highrank_mult_hook.disable();
		m_consumable_hook.disable();
		m_use_item_hook.disable();
		m_ignore_material_hook.disable();
		m_unlock_equipment_hook.disable();

		m_convert_thread_to_fiber_hook.disable();
		m_set_cursor_pos_hook.disable();
		SetWindowLongPtr(g_pointers->m_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_og_wndproc));
		m_swapchain_draw_indexed_hook.disable();
		m_swapchain_resizebuffers_hook.disable();
		m_swapchain_present_hook.disable();
		m_character_ptr_hook.disable();
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

		return g_hooking->m_convert_thread_to_fiber_hook.get_original<decltype(&convert_thread_to_fiber)>()(param);
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

		return g_hooking->m_set_cursor_pos_hook.get_original<decltype(&set_cursor_pos)>()(x, y);
	}
}
