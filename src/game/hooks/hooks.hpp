#pragma once
#include "core/common.hpp"
#include "game/classes/fwddec.hpp"
#include "core/hooking/detour_hook.hpp"

namespace ellohim
{
	class mid_context;
}

// Shellcode stubs implemented in src/core/hooking/return_address.asm
extern "C" void equipment();
extern "C" void player_atk_stat();
extern "C" void player_def_stat();
extern "C" void player_affinity_stat();

namespace big
{
	// Every detour and mid-function callback that belongs to Monster Hunter:
	// World. The framework core only sees these through hook_manager::add().
	struct game_hooks
	{
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
		static int __fastcall player_forward(PlayerForward* a1, __int64 a2);
		static int __fastcall player_position(__int64 a1);
		static int __fastcall waypoint(Waypoint* a1);
		static int WINAPI great_sword_charges(__int64 a1, uint32_t a2, float a3);
		static unsigned int __fastcall glaive_spirit_gauge(__int64 a1, float a2);
		static __int64 __fastcall monster_part_breaking(__int64 a1, unsigned int a2);
		static __int64 __fastcall monster_on_map(__int64 a1, int a2, int a3);
		static int __fastcall bow_gun_effect(__int64 a1);

		// Ellohim mid-hook callbacks (weapon)
		static void mid_gs_charge(ellohim::mid_context& ctx);
		static void mid_spirit_gauge(ellohim::mid_context& ctx);
		static void mid_demon_mode(ellohim::mid_context& ctx);
		static void mid_switch_axe(ellohim::mid_context& ctx);
		static void mid_gl_ammo(ellohim::mid_context& ctx);
		static void mid_gl_cooldown(ellohim::mid_context& ctx);
		static void mid_cb_phials(ellohim::mid_context& ctx);
		static void mid_cb_sword(ellohim::mid_context& ctx);
		static void mid_cb_shield(ellohim::mid_context& ctx);
		static void mid_ig_extract(ellohim::mid_context& ctx);
		static void mid_ig_stamina(ellohim::mid_context& ctx);
		static void mid_ig_jump(ellohim::mid_context& ctx);
		static void mid_bow_coatings(ellohim::mid_context& ctx);
		static void mid_bow_charge(ellohim::mid_context& ctx);
		static void mid_bowgun_no_reload(ellohim::mid_context& ctx);
		static void mid_wyvernblast(ellohim::mid_context& ctx);
		static void mid_wyvernheart(ellohim::mid_context& ctx);
		static void mid_wyvernheart_rapidfire(ellohim::mid_context& ctx);
		static void mid_bowgun_no_recoil(ellohim::mid_context& ctx);

		// Player & misc mid-hook callbacks
		static void mid_part_breaker(ellohim::mid_context& ctx);
		static void mid_mount_reset(ellohim::mid_context& ctx);
		static void mid_frostcraft_drawn(ellohim::mid_context& ctx);
		static void mid_frostcraft_heat(ellohim::mid_context& ctx);
		static void mid_lucky_voucher(ellohim::mid_context& ctx);
		static void mid_load_cannon(ellohim::mid_context& ctx);
		static void mid_pet_mgr(ellohim::mid_context& ctx);
		static void mid_pet_unlocker(ellohim::mid_context& ctx);
		static void mid_palico_gadget_cd(ellohim::mid_context& ctx);
		static void mid_slinger_ammo(ellohim::mid_context& ctx);
		static void mid_maxed_effect_set(ellohim::mid_context& ctx);
		static void mid_maxed_attribute(ellohim::mid_context& ctx);
		static void mid_max_sharpness(ellohim::mid_context& ctx);
		static void mid_max_slot_armor(ellohim::mid_context& ctx);
		static void mid_max_slot_armor_ranged(ellohim::mid_context& ctx);
		static void mid_max_slot_armor_melee(ellohim::mid_context& ctx);
		static void mid_scoutfly_max(ellohim::mid_context& ctx);
		static void mid_scoutfly_level(ellohim::mid_context& ctx);
	};

	// Installs every MHW detour and mid hook. Called by game::initialize() after
	// the pointer table has been resolved and before hook_manager::enable().
	void install_hooks();
}
