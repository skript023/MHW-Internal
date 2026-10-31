#pragma once
#include "core/commands/bool_command.hpp"
#include "core/commands/float_command.hpp"
#include "core/commands/number_command.hpp"
#include "game/features/command_helpers.hpp"

// One command per feature. Each command lives in its own translation unit
// under game/features/<group>/ and registers itself with the command
// registry, so menus bind to it by name (hash) and never touch settings.
namespace big::features
{
	// Writes the cached critical boost stat; called from the player tick.
	void apply_critical_boost();

	// Player
	extern patch_toggle _invulnerable;
	extern patch_toggle _inf_stamina;
	extern patch_toggle _inf_mount;
	extern patch_toggle _inf_gathering;
	extern patch_toggle _inf_cannon_fire;
	extern bool_command _part_breaker;
	extern bool_command _easy_part_breaker;
	extern bool_command _mount_reset;
	extern bool_command _inf_item;
	extern bool_command _reveal_monster;
	extern bool_command _palico_gadget_cd;
	extern bool_command _inf_lucky_voucher;
	extern bool_command _cannon_always_loaded;
	extern bool_command _unlock_all_pet;
	extern bool_command _scoutfly_max_level;
	extern bool_command _max_sharpness;
	extern bool_command _maxed_stat_set;
	extern bool_command _maxed_armor_effect;
	extern bool_command _research_exp_mult;
	extern float_command _critical_boost;
	extern float_command _research_exp;
	extern number_command<int> _hr_ex_mult;
	extern number_command<int> _mr_ex_mult;
	extern number_command<int> _palico_exp_mult;

	// Weapon
	extern patch_toggle _inf_frostcraft;
	extern patch_toggle _inf_slinger_ammo;
	extern bool_command _inst_chrgs_gs;
	extern bool_command _inf_spirit_gauge;
	extern bool_command _inf_demon_mode;
	extern bool_command _full_charged_switch_axe;
	extern bool_command _inf_gl_ammo;
	extern bool_command _no_gl_cooldown;
	extern bool_command _frostcraft_recharge;
	extern bool_command _max_slot_armor;
	extern bool_command _cb_phials;
	extern bool_command _cb_sword_charge;
	extern bool_command _cb_shield_charge;
	extern bool_command _ig_extract;
	extern bool_command _ig_stamina;
	extern bool_command _ig_jump;
	extern bool_command _bow_coatings;
	extern bool_command _bow_charge;
	extern bool_command _bowgun_no_reload;
	extern bool_command _inf_wyvern_blast;
	extern bool_command _inf_wyvernheart;
	extern bool_command _wyvernheart_rapidfire;
	extern bool_command _bowgun_no_recoil;
	extern float_command _frostcraft_drawn;
	extern float_command _frostcraft_heat;

	// Crafting
	extern bool_command _ignore_material;
	extern bool_command _unlock_equipment;

	// Quality of life
	extern bool_command _skippable_cutscenes;
	extern bool_command _post_quest_timer_skip;
	extern bool_command _auto_post_quest_timer_skip;
	extern bool_command _instant_gathering;
	extern bool_command _zero_delay_carving;
	extern bool_command _full_hub_sprint;
	extern bool_command _remote_camp_facilities;
	extern bool_command _quick_quest_restart;
	extern bool_command _tic_rate_fix;
	extern float_command _carve_speed_multiplier;
	extern float_command _hub_speed_multiplier;

	// Overlay / HUD
	extern bool_command _overlay;
	extern bool_command _overlay_monster;
	extern bool_command _overlay_crown;
	extern bool_command _overlay_enrage_bar;
	extern bool_command _overlay_monster_weakness;
	extern bool_command _overlay_hitzone_values;
	extern bool_command _overlay_monster_parts;
	extern bool_command _overlay_monster_ailments;
	extern bool_command _overlay_party_dps;
	extern bool_command _overlay_dropped_materials;
	extern bool_command _light_pillars_shinies;
	extern bool_command _overlay_hunter_buffs;
	extern bool_command _overlay_team_esp;
	extern bool_command _overlay_hunter;

}
