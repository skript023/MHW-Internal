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

		base_hook::add<hooks::set_cursor_pos>(new detour_hook("SetCursorPos", memory::module("user32.dll").get_export("SetCursorPos").as<void*>(), &hooks::set_cursor_pos));
		base_hook::add<hooks::convert_thread_to_fiber>(new detour_hook("ConvertThreadToFiber", memory::module("kernel32.dll").get_export("ConvertThreadToFiber").as<void*>(), &hooks::convert_thread_to_fiber));
		base_hook::add<hooks::material_handler>(new detour_hook("Ignore Material", g_pointers->m_ignore_material, &hooks::material_handler));
		base_hook::add<hooks::equipment_crafting>(new detour_hook("Unlock Equipment", g_pointers->m_equipment_unlock, &hooks::equipment_crafting));
		base_hook::add<hooks::consumable>(new detour_hook("Consumable", g_pointers->m_consumable, &hooks::consumable));
		base_hook::add<hooks::use_item>(new detour_hook("Use Item", g_pointers->m_use_item, &hooks::use_item));
		base_hook::add<hooks::master_rank_experience>(new detour_hook("Master Rank EXP Mult", g_pointers->m_master_rank_exp, &hooks::master_rank_experience));
		base_hook::add<hooks::high_rank_experience>(new detour_hook("High Rank EXP Mult", g_pointers->m_highrank_exp, &hooks::high_rank_experience));
		base_hook::add<hooks::infinite_gathering>(new detour_hook("Inf Gathering", g_pointers->m_inf_gathering, &hooks::infinite_gathering));
		base_hook::add<hooks::research_experience>(new detour_hook("Research EXP Mult", g_pointers->m_research_exp, &hooks::research_experience));
		base_hook::add<hooks::highlighted_armour>(new detour_hook("Equipment Hook", g_pointers->m_armor_ptr, &hooks::highlighted_armour));
		base_hook::add<player_atk_stat>(new detour_hook("Atk Hook", g_pointers->m_atk_ptr, &player_atk_stat));
		//base_hook::add<player_def_stat>(new detour_hook("Def Hook", g_pointers->m_def_ptr, &player_def_stat));
		base_hook::add<player_affinity_stat>(new detour_hook("Aff Hook", g_pointers->m_affinity_ptr, &player_affinity_stat));
		base_hook::add<hooks::deploy_character>(new detour_hook("Character Hook", g_pointers->m_character_ptr, &hooks::deploy_character));
		base_hook::add<hooks::highlighted_weapon>(new detour_hook("Weapon Hook", g_pointers->m_weapon_ptr, &hooks::highlighted_weapon));
		base_hook::add<hooks::meal_handler>(new detour_hook("Meal Handler Hook", g_pointers->m_meal_handler, &hooks::meal_handler));
		base_hook::add<hooks::palico_experience_handler>(new detour_hook("Meal Handler Hook", g_pointers->m_palico_experiece_handler, &hooks::palico_experience_handler));
		base_hook::add<hooks::player_forward>(new detour_hook("Player Coordinates Hook", g_pointers->m_player_forward, &hooks::player_forward));
		base_hook::add<hooks::player_position>(new detour_hook("Player Position Hook", g_pointers->m_player_position, &hooks::player_position));
		base_hook::add<hooks::waypoint>(new detour_hook("Waypoint Position Hook", g_pointers->m_waypoint, &hooks::waypoint));
		base_hook::add<hooks::monster_part_breaking>(new detour_hook("Easy Part Break Hook", g_pointers->m_easy_breaker, &hooks::monster_part_breaking));
		base_hook::add<hooks::monster_on_map>(new detour_hook("Monster On Map Progress Hook", g_pointers->m_reveal_monster, &hooks::monster_on_map));
		base_hook::add<hooks::bow_gun_effect>(new detour_hook("Bow Gun Effect Hook", g_pointers->m_bow_gun_effect, &hooks::bow_gun_effect));

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
