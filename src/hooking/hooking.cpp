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
			auto* gs = reinterpret_cast<GreatswordData*>(ctx.rbx);
			gs->m_charge_level = 3;
			gs->m_charge_timer = 10.0f;
		}
	}

	void hooks::mid_spirit_gauge(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.inf_spirit_gauge)
		{
			auto* ls = reinterpret_cast<LongswordData*>(ctx.rdi);
			ls->m_spirit_gauge = 1.0f;
			ls->m_spirit_level = 3;
			ls->m_spirit_duration = 1.0f;
		}
	}

	void hooks::mid_demon_mode(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.inf_demon_mode)
		{
			auto* db = reinterpret_cast<DualBladesData*>(ctx.rdi);
			db->m_demon_gauge = 1.0f;
			db->m_is_demon_mode = 1;
		}
	}

	void hooks::mid_switch_axe(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.full_charged_switch_axe)
		{
			auto* sa = reinterpret_cast<SwitchAxeData*>(ctx.rcx);
			sa->m_sword_gauge = 100.0f;
			ctx.xmm[6].f32[0] = 100.0f;
		}
	}

	void hooks::mid_gl_ammo(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.inf_gl_ammo)
		{
			auto* gl = reinterpret_cast<GunlanceData*>(ctx.rdx);
			gl->m_ammo = gl->m_max_ammo;
			gl->m_wyrmstake = 1;
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
			auto* cb = reinterpret_cast<ChargeBladeData*>(ctx.rdi);
			cb->m_phials_count = 6;
			cb->m_phials_charge = 0;
		}
	}

	void hooks::mid_cb_sword(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.cb_sword_charge)
		{
			auto* cb = reinterpret_cast<ChargeBladeData*>(ctx.rbx);
			cb->m_sword_charge_timer = 50.0f;
		}
	}

	void hooks::mid_cb_shield(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.cb_shield_charge)
		{
			auto* cb = reinterpret_cast<ChargeBladeData*>(ctx.rbx);
			cb->m_shield_charge_timer = 50.0f;
		}
	}

	void hooks::mid_ig_extract(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.ig_extract)
		{
			auto* ig = reinterpret_cast<InsectGlaiveData*>(ctx.rcx);
			reinterpret_cast<float*>(&ig->m_red_extract_duration)[ctx.rax] = 150.0f;
		}
	}

	void hooks::mid_ig_stamina(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.ig_stamina)
		{
			auto* kinsect = reinterpret_cast<KinsectData*>(ctx.rdi);
			kinsect->m_stamina = 100.0f;
		}
	}

	void hooks::mid_ig_jump(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.ig_jump)
		{
			auto* air = reinterpret_cast<GlaiveAirActionData*>(ctx.rcx);
			air->m_air_dash_count = 0;
		}
	}

	void hooks::mid_bow_coatings(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.bow_coatings)
		{
			auto* bow = reinterpret_cast<BowData*>(ctx.rcx);
			bow->m_coatings = 12;
			ctx.rdx = 12;
		}
	}

	void hooks::mid_bow_charge(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.bow_charge)
		{
			ctx.rdi = 3;
			auto* bow = reinterpret_cast<BowData*>(ctx.rbx);
			bow->m_charge_level = 3;
		}
	}

	void hooks::mid_bowgun_no_reload(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.bowgun_no_reload)
		{
			auto* bg = reinterpret_cast<BowgunData*>(ctx.rdi);
			auto* clip = reinterpret_cast<int32_t*>(&bg->m_active_clips[ctx.rdx]);
			if (*clip <= 1)
				*clip = 2;
		}
	}

	void hooks::mid_wyvernblast(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.inf_wyvern_blast)
		{
			auto* hbg = reinterpret_cast<HeavyBowgunSpecialData*>(ctx.rdi);
			hbg->m_wyvernblast_ammo = 3;
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
			auto* hbg = reinterpret_cast<HeavyBowgunSpecialData*>(ctx.rbx);
			hbg->m_wyvernheart_timer = 0.0f;
			ctx.xmm[0].f32[0] = 0.0f;
		}
	}

	void hooks::mid_bowgun_no_recoil(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.bowgun_no_recoil)
		{
			auto* recoil = reinterpret_cast<BowgunRecoilData*>(ctx.rbx);
			recoil->m_recoil_levels[0] = 4;
			recoil->m_recoil_levels[1] = 4;
			recoil->m_recoil_levels[2] = 3;
			recoil->m_recoil_levels[3] = 3;
			recoil->m_recoil_levels[4] = 3;
			recoil->m_recoil_levels[5] = 4;
			recoil->m_recoil_levels[6] = 1;
			recoil->m_recoil_levels[7] = 3;
			recoil->m_recoil_levels[8] = 1;
			recoil->m_recoil_levels[9] = 1;
			recoil->m_recoil_levels[10] = 1;
			recoil->m_recoil_levels[11] = 1;
			recoil->m_recoil_levels[12] = 1;
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
			auto* mount = reinterpret_cast<MonsterMountData*>(ctx.rcx);
			mount->m_mount_gauge = 0.0f;
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
		{
			auto* voucher = reinterpret_cast<QuestVoucherData*>(ctx.rax);
			voucher->m_lucky_vouchers = 5;
		}
	}

	void hooks::mid_load_cannon(ellohim::mid_context& ctx)
	{
		if (g_settings.player.cannon_always_loaded)
		{
			auto* cannon = reinterpret_cast<SiegeCannonData*>(ctx.rdi);
			cannon->m_cannon_balls = 5;
		}
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
			auto* palico = reinterpret_cast<PalicoGadgetData*>(ctx.rcx);
			palico->m_cooldown = 0.0f;
			ctx.xmm[0].f32[0] = 0.0f;
		}
	}

	void hooks::mid_slinger_ammo(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.inf_slinger_ammo)
		{
			auto* slinger = reinterpret_cast<SlingerData*>(ctx.r9);
			slinger->m_ammo = 99;
		}
	}

	void hooks::mid_maxed_effect_set(ellohim::mid_context& ctx)
	{
		if (g_settings.player.maxed_armor_effect)
		{
			auto* skill = reinterpret_cast<ArmorSkillLevelData*>(ctx.rax);
			skill->m_skill_level = 5;
		}
	}

	void hooks::mid_maxed_attribute(ellohim::mid_context& ctx)
	{
		if (g_settings.player.maxed_stat_set)
		{
			auto* skill = reinterpret_cast<ArmorSkillLevelData*>(ctx.r12 + ctx.rsi);
			skill->m_skill_level = 7;
		}
	}

	void hooks::mid_max_sharpness(ellohim::mid_context& ctx)
	{
		if (g_settings.player.max_sharpness)
		{
			auto* wpn = reinterpret_cast<WeaponSharpnessData*>(ctx.rcx);
			wpn->m_sharpness = 400;
		}
	}

	void hooks::mid_max_slot_armor(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.max_slot_armor)
		{
			auto* armor = reinterpret_cast<ArmorSlotData*>(ctx.r8);
			armor->m_slot_count = 3;
			armor->m_slot_levels[0] = 4;
			armor->m_slot_levels[1] = 4;
			armor->m_slot_levels[2] = 4;
		}
	}

	void hooks::mid_max_slot_armor_ranged(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.max_slot_armor)
		{
			auto* armor = reinterpret_cast<ArmorSlotRanged*>(ctx.r14);
			armor->m_slot_count = 3;
			armor->m_slot_levels[0] = 4;
			armor->m_slot_levels[1] = 4;
			armor->m_slot_levels[2] = 4;
		}
	}

	void hooks::mid_max_slot_armor_melee(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.max_slot_armor)
		{
			auto* armor = reinterpret_cast<ArmorSlotMelee*>(ctx.rsi);
			armor->m_slot_count = 3;
			armor->m_slot_levels[0] = 4;
			armor->m_slot_levels[1] = 4;
			armor->m_slot_levels[2] = 4;
		}
	}

	void hooks::mid_scoutfly_max(ellohim::mid_context& ctx)
	{
		if (g_settings.player.scoutfly_max_level)
		{
			auto* sc = reinterpret_cast<ScoutflyData*>(ctx.rax);
			sc->m_levels[ctx.rbx] = 100.0f;
		}
	}

	void hooks::mid_scoutfly_level(ellohim::mid_context& ctx)
	{
		if (g_settings.player.scoutfly_max_level)
		{
			auto* sc = reinterpret_cast<ScoutflyData*>(ctx.rax);
			sc->m_levels[ctx.rdi] = 100.0f;
		}
	}
}
