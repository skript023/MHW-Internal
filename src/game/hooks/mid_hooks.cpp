#include "game/hooks/hooks.hpp"

#include "game/classes/fwddec.hpp"
#include "game/settings.hpp"

#include <ellohim/hooking/mid_hook.hpp>

namespace big
{
	void game_hooks::mid_gs_charge(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.inst_chrgs_gs)
		{
			ctx.rsi = 3;
			auto* gs = reinterpret_cast<GreatswordData*>(ctx.rbx);
			gs->m_charge_level = 3;
			gs->m_charge_timer = 10.0f;
		}
	}

	void game_hooks::mid_spirit_gauge(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.inf_spirit_gauge)
		{
			auto* ls = reinterpret_cast<LongswordData*>(ctx.rdi);
			ls->m_spirit_gauge = 1.0f;
			ls->m_spirit_level = 3;
			ls->m_spirit_duration = 1.0f;
		}
	}

	void game_hooks::mid_demon_mode(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.inf_demon_mode)
		{
			auto* db = reinterpret_cast<DualBladesData*>(ctx.rdi);
			db->m_demon_gauge = 1.0f;
			db->m_is_demon_mode = 1;
		}
	}

	void game_hooks::mid_switch_axe(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.full_charged_switch_axe)
		{
			auto* sa = reinterpret_cast<SwitchAxeData*>(ctx.rcx);
			sa->m_sword_gauge = 100.0f;
			ctx.xmm[6].f32[0] = 100.0f;
		}
	}

	void game_hooks::mid_gl_ammo(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.inf_gl_ammo)
		{
			auto* gl = reinterpret_cast<GunlanceData*>(ctx.rdx);
			gl->m_ammo = gl->m_max_ammo;
			gl->m_wyrmstake = 1;
		}
	}

	void game_hooks::mid_gl_cooldown(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.no_gl_cooldown)
			ctx.xmm[1].f32[0] = 0.0f;
	}

	void game_hooks::mid_cb_phials(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.cb_phials)
		{
			auto* cb = reinterpret_cast<ChargeBladeData*>(ctx.rdi);
			cb->m_phials_count = 6;
			cb->m_phials_charge = 0;
		}
	}

	void game_hooks::mid_cb_sword(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.cb_sword_charge)
		{
			auto* cb = reinterpret_cast<ChargeBladeData*>(ctx.rbx);
			cb->m_sword_charge_timer = 50.0f;
		}
	}

	void game_hooks::mid_cb_shield(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.cb_shield_charge)
		{
			auto* cb = reinterpret_cast<ChargeBladeData*>(ctx.rbx);
			cb->m_shield_charge_timer = 50.0f;
		}
	}

	void game_hooks::mid_ig_extract(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.ig_extract)
		{
			auto* ig = reinterpret_cast<InsectGlaiveData*>(ctx.rcx);
			reinterpret_cast<float*>(&ig->m_red_extract_duration)[ctx.rax] = 150.0f;
		}
	}

	void game_hooks::mid_ig_stamina(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.ig_stamina)
		{
			auto* kinsect = reinterpret_cast<KinsectData*>(ctx.rdi);
			kinsect->m_stamina = 100.0f;
		}
	}

	void game_hooks::mid_ig_jump(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.ig_jump)
		{
			auto* air = reinterpret_cast<GlaiveAirActionData*>(ctx.rcx);
			air->m_air_dash_count = 0;
		}
	}

	void game_hooks::mid_bow_coatings(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.bow_coatings)
		{
			auto* bow = reinterpret_cast<BowData*>(ctx.rcx);
			bow->m_coatings = 12;
			ctx.rdx = 12;
		}
	}

	void game_hooks::mid_bow_charge(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.bow_charge)
		{
			ctx.rdi = 3;
			auto* bow = reinterpret_cast<BowData*>(ctx.rbx);
			bow->m_charge_level = 3;
		}
	}

	void game_hooks::mid_bowgun_no_reload(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.bowgun_no_reload)
		{
			auto* bg = reinterpret_cast<BowgunData*>(ctx.rdi);
			auto* clip = reinterpret_cast<int32_t*>(&bg->m_active_clips[ctx.rdx]);
			if (*clip <= 1)
				*clip = 2;
		}
	}

	void game_hooks::mid_wyvernblast(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.inf_wyvern_blast)
		{
			auto* hbg = reinterpret_cast<HeavyBowgunSpecialData*>(ctx.rdi);
			hbg->m_wyvernblast_ammo = 3;
			ctx.rcx = 3;
		}
	}

	void game_hooks::mid_wyvernheart(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.inf_wyvernheart)
			ctx.xmm[2].f32[0] = 100.0f;
	}

	void game_hooks::mid_wyvernheart_rapidfire(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.wyvernheart_rapidfire)
		{
			auto* hbg = reinterpret_cast<HeavyBowgunSpecialData*>(ctx.rbx);
			hbg->m_wyvernheart_timer = 0.0f;
			ctx.xmm[0].f32[0] = 0.0f;
		}
	}

	void game_hooks::mid_bowgun_no_recoil(ellohim::mid_context& ctx)
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

	void game_hooks::mid_part_breaker(ellohim::mid_context& ctx)
	{
		if (g_settings.player.part_breaker)
			*reinterpret_cast<float*>(ctx.rsp + 0x1A8) = 990.0f;
	}

	void game_hooks::mid_mount_reset(ellohim::mid_context& ctx)
	{
		if (g_settings.player.mount_reset)
		{
			auto* mount = reinterpret_cast<MonsterMountData*>(ctx.rcx);
			mount->m_mount_gauge = 0.0f;
			ctx.xmm[0].f32[0] = 0.0f;
		}
	}

	void game_hooks::mid_frostcraft_drawn(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.frostcraft_recharge)
			ctx.xmm[0].f32[0] *= g_settings.weapon.frostcraft_drawn;
	}

	void game_hooks::mid_frostcraft_heat(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.frostcraft_recharge)
			ctx.xmm[0].f32[0] *= g_settings.weapon.frostcraft_heat;
	}

	void game_hooks::mid_lucky_voucher(ellohim::mid_context& ctx)
	{
		if (g_settings.player.inf_lucky_voucher)
		{
			auto* voucher = reinterpret_cast<QuestVoucherData*>(ctx.rax);
			voucher->m_lucky_vouchers = 5;
		}
	}

	void game_hooks::mid_load_cannon(ellohim::mid_context& ctx)
	{
		if (g_settings.player.cannon_always_loaded)
		{
			auto* cannon = reinterpret_cast<SiegeCannonData*>(ctx.rdi);
			cannon->m_cannon_balls = 5;
		}
	}

	void game_hooks::mid_pet_mgr(ellohim::mid_context& ctx)
	{
		if (g_settings.player.unlock_all_pet)
			ctx.rax = 99;
	}

	void game_hooks::mid_pet_unlocker(ellohim::mid_context& ctx)
	{
		if (g_settings.player.unlock_all_pet)
			ctx.rax = 99;
	}

	void game_hooks::mid_palico_gadget_cd(ellohim::mid_context& ctx)
	{
		if (g_settings.player.palico_gadget_cd)
		{
			auto* palico = reinterpret_cast<PalicoGadgetData*>(ctx.rcx);
			palico->m_cooldown = 0.0f;
			ctx.xmm[0].f32[0] = 0.0f;
		}
	}

	void game_hooks::mid_slinger_ammo(ellohim::mid_context& ctx)
	{
		if (g_settings.weapon.inf_slinger_ammo)
		{
			auto* slinger = reinterpret_cast<SlingerData*>(ctx.r9);
			slinger->m_ammo = 99;
		}
	}

	void game_hooks::mid_maxed_effect_set(ellohim::mid_context& ctx)
	{
		if (g_settings.player.maxed_armor_effect)
		{
			auto* skill = reinterpret_cast<ArmorSkillLevelData*>(ctx.rax);
			skill->m_skill_level = 5;
		}
	}

	void game_hooks::mid_maxed_attribute(ellohim::mid_context& ctx)
	{
		if (g_settings.player.maxed_stat_set)
		{
			auto* skill = reinterpret_cast<ArmorSkillLevelData*>(ctx.r12 + ctx.rsi);
			skill->m_skill_level = 7;
		}
	}

	void game_hooks::mid_max_sharpness(ellohim::mid_context& ctx)
	{
		if (g_settings.player.max_sharpness)
		{
			auto* wpn = reinterpret_cast<WeaponSharpnessData*>(ctx.rcx);
			wpn->m_sharpness = 400;
		}
	}

	void game_hooks::mid_max_slot_armor(ellohim::mid_context& ctx)
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

	void game_hooks::mid_max_slot_armor_ranged(ellohim::mid_context& ctx)
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

	void game_hooks::mid_max_slot_armor_melee(ellohim::mid_context& ctx)
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

	void game_hooks::mid_scoutfly_max(ellohim::mid_context& ctx)
	{
		if (g_settings.player.scoutfly_max_level)
		{
			auto* sc = reinterpret_cast<ScoutflyData*>(ctx.rax);
			sc->m_levels[ctx.rbx] = 100.0f;
		}
	}

	void game_hooks::mid_scoutfly_level(ellohim::mid_context& ctx)
	{
		if (g_settings.player.scoutfly_max_level)
		{
			auto* sc = reinterpret_cast<ScoutflyData*>(ctx.rax);
			sc->m_levels[ctx.rdi] = 100.0f;
		}
	}
}
