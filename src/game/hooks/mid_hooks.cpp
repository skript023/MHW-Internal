#include "game/features/features.hpp"
#include "game/hooks/hooks.hpp"

#include "game/classes/fwddec.hpp"

#include "core/hooking/mid_hook.hpp"

namespace big
{
	void game_hooks::mid_gs_charge(mid_context& ctx)
	{
		if (features::_inst_chrgs_gs.get_state())
		{
			ctx.rsi = 3;
			auto* gs = reinterpret_cast<GreatswordData*>(ctx.rbx);
			gs->m_charge_level = 3;
			gs->m_charge_timer = 10.0f;
		}
	}

	void game_hooks::mid_spirit_gauge(mid_context& ctx)
	{
		if (features::_inf_spirit_gauge.get_state())
		{
			auto* ls = reinterpret_cast<LongswordData*>(ctx.rdi);
			ls->m_spirit_gauge = 1.0f;
			ls->m_spirit_level = 3;
			ls->m_spirit_duration = 1.0f;
		}
	}

	void game_hooks::mid_demon_mode(mid_context& ctx)
	{
		if (features::_inf_demon_mode.get_state())
		{
			auto* db = reinterpret_cast<DualBladesData*>(ctx.rdi);
			db->m_demon_gauge = 1.0f;
			db->m_is_demon_mode = 1;
		}
	}

	void game_hooks::mid_switch_axe(mid_context& ctx)
	{
		if (features::_full_charged_switch_axe.get_state())
		{
			auto* sa = reinterpret_cast<SwitchAxeData*>(ctx.rcx);
			sa->m_sword_gauge = 100.0f;
			ctx.xmm[6].f32[0] = 100.0f;
		}
	}

	void game_hooks::mid_gl_ammo(mid_context& ctx)
	{
		if (features::_inf_gl_ammo.get_state())
		{
			auto* gl = reinterpret_cast<GunlanceData*>(ctx.rdx);
			gl->m_ammo = gl->m_max_ammo;
			gl->m_wyrmstake = 1;
		}
	}

	void game_hooks::mid_gl_cooldown(mid_context& ctx)
	{
		if (features::_no_gl_cooldown.get_state())
			ctx.xmm[1].f32[0] = 0.0f;
	}

	void game_hooks::mid_cb_phials(mid_context& ctx)
	{
		if (features::_cb_phials.get_state())
		{
			auto* cb = reinterpret_cast<ChargeBladeData*>(ctx.rdi);
			cb->m_phials_count = 6;
			cb->m_phials_charge = 0;
		}
	}

	void game_hooks::mid_cb_sword(mid_context& ctx)
	{
		if (features::_cb_sword_charge.get_state())
		{
			auto* cb = reinterpret_cast<ChargeBladeData*>(ctx.rbx);
			cb->m_sword_charge_timer = 50.0f;
		}
	}

	void game_hooks::mid_cb_shield(mid_context& ctx)
	{
		if (features::_cb_shield_charge.get_state())
		{
			auto* cb = reinterpret_cast<ChargeBladeData*>(ctx.rbx);
			cb->m_shield_charge_timer = 50.0f;
		}
	}

	void game_hooks::mid_ig_extract(mid_context& ctx)
	{
		if (features::_ig_extract.get_state())
		{
			auto* ig = reinterpret_cast<InsectGlaiveData*>(ctx.rcx);
			reinterpret_cast<float*>(&ig->m_red_extract_duration)[ctx.rax] = 150.0f;
		}
	}

	void game_hooks::mid_ig_stamina(mid_context& ctx)
	{
		if (features::_ig_stamina.get_state())
		{
			auto* kinsect = reinterpret_cast<KinsectData*>(ctx.rdi);
			kinsect->m_stamina = 100.0f;
		}
	}

	void game_hooks::mid_ig_jump(mid_context& ctx)
	{
		if (features::_ig_jump.get_state())
		{
			auto* air = reinterpret_cast<GlaiveAirActionData*>(ctx.rcx);
			air->m_air_dash_count = 0;
		}
	}

	void game_hooks::mid_bow_coatings(mid_context& ctx)
	{
		if (features::_bow_coatings.get_state())
		{
			auto* bow = reinterpret_cast<BowData*>(ctx.rcx);
			bow->m_coatings = 12;
			ctx.rdx = 12;
		}
	}

	void game_hooks::mid_bow_charge(mid_context& ctx)
	{
		if (features::_bow_charge.get_state())
		{
			ctx.rdi = 3;
			auto* bow = reinterpret_cast<BowData*>(ctx.rbx);
			bow->m_charge_level = 3;
		}
	}

	void game_hooks::mid_bowgun_no_reload(mid_context& ctx)
	{
		if (features::_bowgun_no_reload.get_state())
		{
			auto* bg = reinterpret_cast<BowgunData*>(ctx.rdi);
			auto* clip = reinterpret_cast<int32_t*>(&bg->m_active_clips[ctx.rdx]);
			if (*clip <= 1)
				*clip = 2;
		}
	}

	void game_hooks::mid_wyvernblast(mid_context& ctx)
	{
		if (features::_inf_wyvern_blast.get_state())
		{
			auto* hbg = reinterpret_cast<HeavyBowgunSpecialData*>(ctx.rdi);
			hbg->m_wyvernblast_ammo = 3;
			ctx.rcx = 3;
		}
	}

	void game_hooks::mid_wyvernheart(mid_context& ctx)
	{
		if (features::_inf_wyvernheart.get_state())
			ctx.xmm[2].f32[0] = 100.0f;
	}

	void game_hooks::mid_wyvernheart_rapidfire(mid_context& ctx)
	{
		if (features::_wyvernheart_rapidfire.get_state())
		{
			auto* hbg = reinterpret_cast<HeavyBowgunSpecialData*>(ctx.rbx);
			hbg->m_wyvernheart_timer = 0.0f;
			ctx.xmm[0].f32[0] = 0.0f;
		}
	}

	void game_hooks::mid_bowgun_no_recoil(mid_context& ctx)
	{
		if (features::_bowgun_no_recoil.get_state())
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

	void game_hooks::mid_part_breaker(mid_context& ctx)
	{
		if (features::_part_breaker.get_state())
			*reinterpret_cast<float*>(ctx.rsp + 0x1A8) = 990.0f;
	}

	void game_hooks::mid_mount_reset(mid_context& ctx)
	{
		if (features::_mount_reset.get_state())
		{
			auto* mount = reinterpret_cast<MonsterMountData*>(ctx.rcx);
			mount->m_mount_gauge = 0.0f;
			ctx.xmm[0].f32[0] = 0.0f;
		}
	}

	void game_hooks::mid_frostcraft_drawn(mid_context& ctx)
	{
		if (features::_frostcraft_recharge.get_state())
			ctx.xmm[0].f32[0] *= features::_frostcraft_drawn.get_state();
	}

	void game_hooks::mid_frostcraft_heat(mid_context& ctx)
	{
		if (features::_frostcraft_recharge.get_state())
			ctx.xmm[0].f32[0] *= features::_frostcraft_heat.get_state();
	}

	void game_hooks::mid_lucky_voucher(mid_context& ctx)
	{
		if (features::_inf_lucky_voucher.get_state())
		{
			auto* voucher = reinterpret_cast<QuestVoucherData*>(ctx.rax);
			voucher->m_lucky_vouchers = 5;
		}
	}

	void game_hooks::mid_load_cannon(mid_context& ctx)
	{
		if (features::_cannon_always_loaded.get_state())
		{
			auto* cannon = reinterpret_cast<SiegeCannonData*>(ctx.rdi);
			cannon->m_cannon_balls = 5;
		}
	}

	void game_hooks::mid_pet_mgr(mid_context& ctx)
	{
		if (features::_unlock_all_pet.get_state())
			ctx.rax = 99;
	}

	void game_hooks::mid_pet_unlocker(mid_context& ctx)
	{
		if (features::_unlock_all_pet.get_state())
			ctx.rax = 99;
	}

	void game_hooks::mid_palico_gadget_cd(mid_context& ctx)
	{
		if (features::_palico_gadget_cd.get_state())
		{
			auto* palico = reinterpret_cast<PalicoGadgetData*>(ctx.rcx);
			palico->m_cooldown = 0.0f;
			ctx.xmm[0].f32[0] = 0.0f;
		}
	}

	void game_hooks::mid_slinger_ammo(mid_context& ctx)
	{
		if (features::_inf_slinger_ammo.get_state())
		{
			auto* slinger = reinterpret_cast<SlingerData*>(ctx.r9);
			slinger->m_ammo = 99;
		}
	}

	void game_hooks::mid_maxed_effect_set(mid_context& ctx)
	{
		if (features::_maxed_armor_effect.get_state())
		{
			auto* skill = reinterpret_cast<ArmorSkillLevelData*>(ctx.rax);
			skill->m_skill_level = 5;
		}
	}

	void game_hooks::mid_maxed_attribute(mid_context& ctx)
	{
		if (features::_maxed_stat_set.get_state())
		{
			auto* skill = reinterpret_cast<ArmorSkillLevelData*>(ctx.r12 + ctx.rsi);
			skill->m_skill_level = 7;
		}
	}

	void game_hooks::mid_max_sharpness(mid_context& ctx)
	{
		if (features::_max_sharpness.get_state())
		{
			auto* wpn = reinterpret_cast<WeaponSharpnessData*>(ctx.rcx);
			wpn->m_sharpness = 400;
		}
	}

	void game_hooks::mid_max_slot_armor(mid_context& ctx)
	{
		if (features::_max_slot_armor.get_state())
		{
			auto* armor = reinterpret_cast<ArmorSlotData*>(ctx.r8);
			armor->m_slot_count = 3;
			armor->m_slot_levels[0] = 4;
			armor->m_slot_levels[1] = 4;
			armor->m_slot_levels[2] = 4;
		}
	}

	void game_hooks::mid_max_slot_armor_ranged(mid_context& ctx)
	{
		if (features::_max_slot_armor.get_state())
		{
			auto* armor = reinterpret_cast<ArmorSlotRanged*>(ctx.r14);
			armor->m_slot_count = 3;
			armor->m_slot_levels[0] = 4;
			armor->m_slot_levels[1] = 4;
			armor->m_slot_levels[2] = 4;
		}
	}

	void game_hooks::mid_max_slot_armor_melee(mid_context& ctx)
	{
		if (features::_max_slot_armor.get_state())
		{
			auto* armor = reinterpret_cast<ArmorSlotMelee*>(ctx.rsi);
			armor->m_slot_count = 3;
			armor->m_slot_levels[0] = 4;
			armor->m_slot_levels[1] = 4;
			armor->m_slot_levels[2] = 4;
		}
	}

	void game_hooks::mid_scoutfly_max(mid_context& ctx)
	{
		if (features::_scoutfly_max_level.get_state())
		{
			auto* sc = reinterpret_cast<ScoutflyData*>(ctx.rax);
			sc->m_levels[ctx.rbx] = 100.0f;
		}
	}

	void game_hooks::mid_scoutfly_level(mid_context& ctx)
	{
		if (features::_scoutfly_max_level.get_state())
		{
			auto* sc = reinterpret_cast<ScoutflyData*>(ctx.rax);
			sc->m_levels[ctx.rdi] = 100.0f;
		}
	}
}
