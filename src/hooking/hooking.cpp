#include "hooking.hpp"

#include "common.hpp"
#include "memory/module.hpp"
#include "pointers.hpp"
#include "renderer.hpp"
#include "gui.hpp"
#include "menu_settings.hpp"
#include <astra/host/canvas.hpp>

#include <ellohim/logger.hpp>
#include <ellohim/hooking/mid_hook.hpp>

namespace big
{
	hooking::hooking()
	{
		ellohim::logger::set_callback([](ellohim::log_level level, std::string_view message) {
			if (level == ellohim::log_level::info)
			{
				LOG(INFO) << "[Ellohim] " << message;
			}
			else
			{
				LOG(WARNING) << "[Ellohim] " << message;
			}
		});

		detour_hook::add<hooks::swapchain_present>("SwapChainPresent", g_pointers->m_swapchain_methods[hooks::swapchain_present_index]);
		detour_hook::add<hooks::swapchain_resizebuffers>("SwapChainResizeBuffers", g_pointers->m_swapchain_methods[hooks::swapchain_resizebuffers_index]);
		if (g_pointers->m_command_queue_execute_command_lists)
		{
			detour_hook::add<hooks::execute_command_lists>("ExecuteCommandLists", g_pointers->m_command_queue_execute_command_lists);
		}

		detour_hook::add<hooks::set_cursor_pos>("SetCursorPos", memory::module("user32.dll").get_export("SetCursorPos").as<void*>());
		detour_hook::add<hooks::convert_thread_to_fiber>(
		    "ConvertThreadToFiber", memory::module("kernel32.dll").get_export("ConvertThreadToFiber").as<void*>());
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
		detour_hook::add<player_affinity_stat>("Aff Hook", g_pointers->m_affinity_ptr);
		detour_hook::add<hooks::deploy_character>("Character Hook", g_pointers->m_character_ptr);
		detour_hook::add<hooks::highlighted_weapon>("Weapon Hook", g_pointers->m_weapon_ptr);
		detour_hook::add<hooks::meal_handler>("Meal Handler Hook", g_pointers->m_meal_handler);
		detour_hook::add<hooks::palico_experience_handler>("Palico Exp Handler Hook", g_pointers->m_palico_experiece_handler);
		detour_hook::add<hooks::player_forward>("Player Coordinates Hook", g_pointers->m_player_forward);
		detour_hook::add<hooks::player_position>("Player Position Hook", g_pointers->m_player_position);
		detour_hook::add<hooks::waypoint>("Waypoint Position Hook", g_pointers->m_waypoint);
		detour_hook::add<hooks::monster_part_breaking>("Easy Part Break Hook", g_pointers->m_easy_breaker);
		detour_hook::add<hooks::monster_on_map>("Monster On Map Progress Hook", g_pointers->m_reveal_monster);
		detour_hook::add<hooks::bow_gun_effect>("Bow Gun Effect Hook", g_pointers->m_bow_gun_effect);

		// Weapon mid-hooks (Safi Full V2)
		if (g_pointers->m_gs_inst_chrgs)
			ellohim::mid_hook::add<hooks::mid_gs_charge>("Instantly Charged GS", g_pointers->m_gs_inst_chrgs);
		if (g_pointers->m_spirit_gauge)
			ellohim::mid_hook::add<hooks::mid_spirit_gauge>("Inf Spirit Gauge", g_pointers->m_spirit_gauge);
		if (g_pointers->m_demon_mode)
			ellohim::mid_hook::add<hooks::mid_demon_mode>("Inf Demon Mode", g_pointers->m_demon_mode);
		if (g_pointers->m_fully_charged_switch_axe)
			ellohim::mid_hook::add<hooks::mid_switch_axe>("Fully Charged Switch Axe", g_pointers->m_fully_charged_switch_axe);
		if (g_pointers->m_gl_ammo)
			ellohim::mid_hook::add<hooks::mid_gl_ammo>("Inf GL Ammo/Wyrmstake", g_pointers->m_gl_ammo);
		if (g_pointers->m_gl_cooldown)
			ellohim::mid_hook::add<hooks::mid_gl_cooldown>("No GL Cooldown", g_pointers->m_gl_cooldown);
		if (g_pointers->m_charged_blade_phials)
			ellohim::mid_hook::add<hooks::mid_cb_phials>("Inf CB Phials", g_pointers->m_charged_blade_phials);
		if (g_pointers->m_cb_sword)
			ellohim::mid_hook::add<hooks::mid_cb_sword>("Inf CB Sword Charge", g_pointers->m_cb_sword);
		if (g_pointers->m_cb_shield)
			ellohim::mid_hook::add<hooks::mid_cb_shield>("Inf CB Shield Charge", g_pointers->m_cb_shield);
		if (g_pointers->m_ig_extract)
			ellohim::mid_hook::add<hooks::mid_ig_extract>("Inf Extract Duration", g_pointers->m_ig_extract);
		if (g_pointers->m_ig_stamina)
			ellohim::mid_hook::add<hooks::mid_ig_stamina>("Inf IG Stamina", g_pointers->m_ig_stamina);
		if (g_pointers->m_ig_jump)
			ellohim::mid_hook::add<hooks::mid_ig_jump>("Inf IG Jumping Attack", g_pointers->m_ig_jump);
		if (g_pointers->m_bow_coatings)
			ellohim::mid_hook::add<hooks::mid_bow_coatings>("Bows Support All Coatings", g_pointers->m_bow_coatings);
		if (g_pointers->m_bow_gun_charge)
			ellohim::mid_hook::add<hooks::mid_bow_charge>("Max Bow Charge", g_pointers->m_bow_gun_charge);
		if (g_pointers->m_bowgun_no_reload)
			ellohim::mid_hook::add<hooks::mid_bowgun_no_reload>("Bowgun No Reload", g_pointers->m_bowgun_no_reload);
		if (g_pointers->m_inf_wyvern_blast)
			ellohim::mid_hook::add<hooks::mid_wyvernblast>("Inf Wyvernblast", g_pointers->m_inf_wyvern_blast);
		if (g_pointers->m_wyvernheart)
			ellohim::mid_hook::add<hooks::mid_wyvernheart>("Inf Wyvernheart", g_pointers->m_wyvernheart);
		if (g_pointers->m_wyvernheart_rapidfire)
			ellohim::mid_hook::add<hooks::mid_wyvernheart_rapidfire>("Wyvernheart Rapidfire", g_pointers->m_wyvernheart_rapidfire);
		if (g_pointers->m_bowgun_no_recoil)
			ellohim::mid_hook::add<hooks::mid_bowgun_no_recoil>("Bowgun No Recoil", g_pointers->m_bowgun_no_recoil);

		// Player & Misc mid-hooks
		if (g_pointers->m_part_breaker)
			ellohim::mid_hook::add<hooks::mid_part_breaker>("Part Breaker", g_pointers->m_part_breaker);
		if (g_pointers->m_mount_reset)
			ellohim::mid_hook::add<hooks::mid_mount_reset>("Mount Reset", g_pointers->m_mount_reset);
		if (g_pointers->m_frostcraft_drawn)
			ellohim::mid_hook::add<hooks::mid_frostcraft_drawn>("Frostcraft Drawn", g_pointers->m_frostcraft_drawn);
		if (g_pointers->m_frostcraft_heat)
			ellohim::mid_hook::add<hooks::mid_frostcraft_heat>("Frostcraft Heat", g_pointers->m_frostcraft_heat);
		if (g_pointers->m_lucky_voucher)
			ellohim::mid_hook::add<hooks::mid_lucky_voucher>("Lucky Voucher", g_pointers->m_lucky_voucher);
		if (g_pointers->m_loaded_cannon)
			ellohim::mid_hook::add<hooks::mid_load_cannon>("Loaded Cannon", g_pointers->m_loaded_cannon);
		if (g_pointers->m_pet_mgr)
			ellohim::mid_hook::add<hooks::mid_pet_mgr>("Pet Manager", g_pointers->m_pet_mgr);
		if (g_pointers->m_pet_unlocker)
			ellohim::mid_hook::add<hooks::mid_pet_unlocker>("Pet Unlocker", g_pointers->m_pet_unlocker);
		if (g_pointers->m_palico_gadget_cd)
			ellohim::mid_hook::add<hooks::mid_palico_gadget_cd>("Palico Gadget CD", g_pointers->m_palico_gadget_cd);
		if (g_pointers->m_slinger_ammo)
			ellohim::mid_hook::add<hooks::mid_slinger_ammo>("Slinger Ammo", g_pointers->m_slinger_ammo);
		if (g_pointers->m_maxed_effect_set)
			ellohim::mid_hook::add<hooks::mid_maxed_effect_set>("Maxed Set Effect", g_pointers->m_maxed_effect_set);
		if (g_pointers->m_maxed_attribute)
			ellohim::mid_hook::add<hooks::mid_maxed_attribute>("Maxed Armor Attribute", g_pointers->m_maxed_attribute);
		if (g_pointers->m_max_sharpness)
			ellohim::mid_hook::add<hooks::mid_max_sharpness>("Max Sharpness", g_pointers->m_max_sharpness);
		if (g_pointers->m_max_slot_armor)
			ellohim::mid_hook::add<hooks::mid_max_slot_armor>("Max Slot Armor", g_pointers->m_max_slot_armor);
		if (g_pointers->m_max_slot_armor_ranged)
			ellohim::mid_hook::add<hooks::mid_max_slot_armor_ranged>("Max Slot Armor Ranged", g_pointers->m_max_slot_armor_ranged);
		if (g_pointers->m_max_slot_armor_melee)
			ellohim::mid_hook::add<hooks::mid_max_slot_armor_melee>("Max Slot Armor Melee", g_pointers->m_max_slot_armor_melee);
		if (g_pointers->m_scoutfly_max)
			ellohim::mid_hook::add<hooks::mid_scoutfly_max>("Scoutfly Max", g_pointers->m_scoutfly_max);
		if (g_pointers->m_scoutfly_level)
			ellohim::mid_hook::add<hooks::mid_scoutfly_level>("Scoutfly Level", g_pointers->m_scoutfly_level);

		g_hooking = this;
	}

	hooking::~hooking()
	{
		if (m_enabled)
			disable();

		ellohim::mid_hook::destroy_owned();
		delete_disabled_hooks();

		g_hooking = nullptr;
	}

	void hooking::delete_disabled_hooks()
	{
		std::vector<detour_base*> disabled;
		for (auto* hook : detour_base::hooks())
		{
			if (!hook->is_enabled())
			{
				disabled.push_back(hook);
			}
		}
		for (auto* hook : disabled)
		{
			delete hook;
		}
	}

	void hooking::enable()
	{
		m_og_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(g_pointers->m_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&hooks::wndproc)));

		try
		{
			ellohim::mid_hook::enable_all();
			if (!detour_base::enable_all())
			{
				throw std::runtime_error("Could not enable all hooks");
			}
			m_enabled = true;
		}
		catch (...)
		{
			SetWindowLongPtrW(g_pointers->m_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_og_wndproc));
			detour_base::disable_all();
		ellohim::mid_hook::disable_all();
			throw;
		}
	}

	void hooking::disable()
	{
		m_enabled = false;
		SetWindowLongPtrW(g_pointers->m_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_og_wndproc));
		detour_base::disable_all();
		ellohim::mid_hook::disable_all();
		for (auto* hook : detour_base::hooks())
		{
			if (hook->is_enabled())
			{
				LOG(WARNING) << "Hook '" << hook->name() << "' is covered by another program's hook; it stays in place.";
			}
		}
		delete_disabled_hooks();
	}

	void* hooks::convert_thread_to_fiber(void* param)
	{
		if (IsThreadAFiber())
		{
			return GetCurrentFiber();
		}

		return base_hook::get_original<hooks::convert_thread_to_fiber>()(param);
	}

	LRESULT hooks::wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (g_running)
		{
			if (g_renderer)
				g_renderer->wndproc(hwnd, msg, wparam, lparam);
			if (auto result = canvas::block_game_message(hwnd, msg, wparam, lparam))
			{
				return *result;
			}
		}

		return CallWindowProcW(g_hooking->m_og_wndproc, hwnd, msg, wparam, lparam);
	}

	BOOL hooks::set_cursor_pos(int x, int y)
	{
		if (canvas::uses_mouse())
			return true;

		return base_hook::get_original<hooks::set_cursor_pos>()(x, y);
	}
}


namespace big
{
	void hooks::mid_gs_charge(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.inst_chrgs_gs)
		{
			ctx.rsi = 3;
			*reinterpret_cast<uint32_t*>(ctx.rbx + 0x2358) = 3;
			*reinterpret_cast<float*>(ctx.rbx + 0x2350) = 10.0f;
		}
	}

	void hooks::mid_spirit_gauge(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.inf_spirit_gauge)
		{
			*reinterpret_cast<float*>(ctx.rdi + 0x2368) = 1.0f;
			*reinterpret_cast<uint32_t*>(ctx.rdi + 0x2370) = 3;
			*reinterpret_cast<float*>(ctx.rdi + 0x2374) = 1.0f;
		}
	}

	void hooks::mid_demon_mode(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.inf_demon_mode)
		{
			*reinterpret_cast<float*>(ctx.rdi + 0x236C) = 1.0f;
			*reinterpret_cast<uint8_t*>(ctx.rdi + 0x2369) = 1;
		}
	}

	void hooks::mid_switch_axe(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.full_charged_switch_axe)
		{
			*reinterpret_cast<float*>(ctx.rcx + 0x2350) = 100.0f;
			ctx.xmm[6].f32[0] = 100.0f;
		}
	}

	void hooks::mid_gl_ammo(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.inf_gl_ammo)
		{
			*reinterpret_cast<uint32_t*>(ctx.rdx + 0x236C) = *reinterpret_cast<uint32_t*>(ctx.rdx + 0x2368);
			*reinterpret_cast<uint32_t*>(ctx.rdx + 0x2378) = 1;
		}
	}

	void hooks::mid_gl_cooldown(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.no_gl_cooldown)
			ctx.xmm[1].f32[0] = 0.0f;
	}

	void hooks::mid_cb_phials(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.cb_phials)
		{
			*reinterpret_cast<uint32_t*>(ctx.rdi + 0x2374) = 6;
			*reinterpret_cast<uint16_t*>(ctx.rdi + 0x2372) = 0;
		}
	}

	void hooks::mid_cb_sword(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.cb_sword_charge)
			*reinterpret_cast<float*>(ctx.rbx + 0x237C) = 50.0f;
	}

	void hooks::mid_cb_shield(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.cb_shield_charge)
			*reinterpret_cast<float*>(ctx.rbx + 0x2378) = 50.0f;
	}

	void hooks::mid_ig_extract(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.ig_extract)
			*reinterpret_cast<float*>(ctx.rcx + ctx.rax * 4 + 0x2368) = 150.0f;
	}

	void hooks::mid_ig_stamina(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.ig_stamina)
			*reinterpret_cast<float*>(ctx.rdi + 0x0A8C) = 100.0f;
	}

	void hooks::mid_ig_jump(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.ig_jump)
			*reinterpret_cast<uint32_t*>(ctx.rcx + 0x2428) = 0;
	}

	void hooks::mid_bow_coatings(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.bow_coatings)
		{
			*reinterpret_cast<uint8_t*>(ctx.rcx + 0x45) = 12;
			ctx.rdx = 12;
		}
	}

	void hooks::mid_bow_charge(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.bow_charge)
		{
			ctx.rdi = 3;
			*reinterpret_cast<uint32_t*>(ctx.rbx + 0x23B4) = 3;
		}
	}

	void hooks::mid_bowgun_no_reload(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.bowgun_no_reload)
		{
			auto* clip = reinterpret_cast<int32_t*>(ctx.rdi + ctx.rdx * 8 + 0x23A8);
			if (*clip <= 1)
				*clip = 2;
		}
	}

	void hooks::mid_wyvernblast(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.inf_wyvern_blast)
		{
			*reinterpret_cast<uint32_t*>(ctx.rdi + 0x2848) = 3;
			ctx.rcx = 3;
		}
	}

	void hooks::mid_wyvernheart(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.inf_wyvernheart)
			ctx.xmm[2].f32[0] = 100.0f;
	}

	void hooks::mid_wyvernheart_rapidfire(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.wyvernheart_rapidfire)
		{
			*reinterpret_cast<float*>(ctx.rbx + 0x09C4) = 0.0f;
			ctx.xmm[0].f32[0] = 0.0f;
		}
	}

	void hooks::mid_bowgun_no_recoil(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.bowgun_no_recoil)
		{
			*reinterpret_cast<int32_t*>(ctx.rbx + 0x2368 + 0x00) = 4;
			*reinterpret_cast<int32_t*>(ctx.rbx + 0x2368 + 0x04) = 4;
			*reinterpret_cast<int32_t*>(ctx.rbx + 0x2368 + 0x08) = 3;
			*reinterpret_cast<int32_t*>(ctx.rbx + 0x2368 + 0x0C) = 3;
			*reinterpret_cast<int32_t*>(ctx.rbx + 0x2368 + 0x10) = 3;
			*reinterpret_cast<int32_t*>(ctx.rbx + 0x2368 + 0x14) = 4;
			*reinterpret_cast<int32_t*>(ctx.rbx + 0x2368 + 0x18) = 1;
			*reinterpret_cast<int32_t*>(ctx.rbx + 0x2368 + 0x1C) = 3;
			*reinterpret_cast<int32_t*>(ctx.rbx + 0x2368 + 0x20) = 1;
			*reinterpret_cast<int32_t*>(ctx.rbx + 0x2368 + 0x24) = 1;
			*reinterpret_cast<int32_t*>(ctx.rbx + 0x2368 + 0x28) = 1;
			*reinterpret_cast<int32_t*>(ctx.rbx + 0x2368 + 0x2C) = 1;
			*reinterpret_cast<int32_t*>(ctx.rbx + 0x2368 + 0x30) = 1;
		}
	}

	void hooks::mid_part_breaker(ellohim::mid_context& ctx)
	{
		if (g_settings.player.part_breaker)
			*reinterpret_cast<float*>(ctx.rsp + 0x1A8) = 990.0f;
	}

	void hooks::mid_mount_reset(ellohim::mid_context& ctx)
	{
		if (g_settings.player.mount_reset)
		{
			*reinterpret_cast<float*>(ctx.rcx + 0x1C170) = 0.0f;
			ctx.xmm[0].f32[0] = 0.0f;
		}
	}

	void hooks::mid_frostcraft_drawn(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.frostcraft_recharge)
			ctx.xmm[0].f32[0] *= g_settings.weapon.frostcraft_drawn;
	}

	void hooks::mid_frostcraft_heat(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.frostcraft_recharge)
			ctx.xmm[0].f32[0] *= g_settings.weapon.frostcraft_heat;
	}

	void hooks::mid_lucky_voucher(ellohim::mid_context& ctx)
	{
		if (g_settings.player.inf_lucky_voucher)
			*reinterpret_cast<uint32_t*>(ctx.rax + 0x269154) = 5;
	}

	void hooks::mid_load_cannon(ellohim::mid_context& ctx)
	{
		if (g_settings.player.cannon_always_loaded)
			*reinterpret_cast<uint32_t*>(ctx.rdi + 0x0FB8) = 5;
	}

	void hooks::mid_pet_mgr(ellohim::mid_context& ctx)
	{
		if (g_settings.player.unlock_all_pet)
			ctx.rax = 99;
	}

	void hooks::mid_pet_unlocker(ellohim::mid_context& ctx)
	{
		if (g_settings.player.unlock_all_pet)
			ctx.rax = 99;
	}

	void hooks::mid_palico_gadget_cd(ellohim::mid_context& ctx)
	{
		if (g_settings.player.palico_gadget_cd)
		{
			*reinterpret_cast<float*>(ctx.rcx + 0xA340) = 0.0f;
			ctx.xmm[0].f32[0] = 0.0f;
		}
	}

	void hooks::mid_slinger_ammo(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.inf_slinger_ammo)
			*reinterpret_cast<uint32_t*>(ctx.r9 + 0x7E20) = 99;
	}

	void hooks::mid_maxed_effect_set(ellohim::mid_context& ctx)
	{
		if (g_settings.player.maxed_armor_effect)
			*reinterpret_cast<uint8_t*>(ctx.rax + 0x31) = 5;
	}

	void hooks::mid_maxed_attribute(ellohim::mid_context& ctx)
	{
		if (g_settings.player.maxed_stat_set)
			*reinterpret_cast<uint8_t*>(ctx.r12 + ctx.rsi + 0x31) = 7;
	}

	void hooks::mid_max_sharpness(ellohim::mid_context& ctx)
	{
		if (g_settings.player.max_sharpness)
			*reinterpret_cast<uint32_t*>(ctx.rcx + 0x20F8) = 400;
	}

	void hooks::mid_max_slot_armor(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.max_slot_armor)
		{
			*reinterpret_cast<uint8_t*>(ctx.r8 + 0x11) = 3;
			*reinterpret_cast<uint8_t*>(ctx.r8 + 0x40) = 4;
			*reinterpret_cast<uint8_t*>(ctx.r8 + 0x41) = 4;
			*reinterpret_cast<uint8_t*>(ctx.r8 + 0x42) = 4;
		}
	}

	void hooks::mid_max_slot_armor_ranged(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.max_slot_armor)
		{
			*reinterpret_cast<uint8_t*>(ctx.r14 + 0x33) = 3;
			*reinterpret_cast<uint8_t*>(ctx.r14 + 0x34) = 4;
			*reinterpret_cast<uint8_t*>(ctx.r14 + 0x35) = 4;
			*reinterpret_cast<uint8_t*>(ctx.r14 + 0x36) = 4;
		}
	}

	void hooks::mid_max_slot_armor_melee(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.max_slot_armor)
		{
			*reinterpret_cast<uint8_t*>(ctx.rsi + 0x2D) = 3;
			*reinterpret_cast<uint8_t*>(ctx.rsi + 0x2E) = 4;
			*reinterpret_cast<uint8_t*>(ctx.rsi + 0x2F) = 4;
			*reinterpret_cast<uint8_t*>(ctx.rsi + 0x30) = 4;
		}
	}

	void hooks::mid_scoutfly_max(ellohim::mid_context& ctx)
	{
		if (g_settings.player.scoutfly_max_level)
			*reinterpret_cast<float*>(ctx.rax + ctx.rbx * 4 + 0x0F62A8) = 100.0f;
	}

	void hooks::mid_scoutfly_level(ellohim::mid_context& ctx)
	{
		if (g_settings.player.scoutfly_max_level)
			*reinterpret_cast<float*>(ctx.rax + ctx.rdi * 4 + 0x0F62A8) = 100.0f;
	}
}
