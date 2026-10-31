#include "game/hooks/hooks.hpp"

#include "core/hooking/hook_manager.hpp"
#include "game/pointers.hpp"

#include "core/hooking/mid_hook.hpp"

namespace big
{
	// Game-owned detours and mid-function hooks. Registered by game::initialize()
	// once the pointer table is resolved; enabled by hook_manager::enable() and
	// mid_hook::enable_all().
	void install_hooks()
	{
		hook_manager::add<game_hooks::material_handler>("Ignore Material", g_pointers->m_ignore_material);
		hook_manager::add<game_hooks::equipment_crafting>("Unlock Equipment", g_pointers->m_equipment_unlock);
		hook_manager::add<game_hooks::consumable>("Consumable", g_pointers->m_consumable);
		hook_manager::add<game_hooks::use_item>("Use Item", g_pointers->m_use_item);
		hook_manager::add<game_hooks::master_rank_experience>("Master Rank EXP Mult", g_pointers->m_master_rank_exp);
		hook_manager::add<game_hooks::high_rank_experience>("High Rank EXP Mult", g_pointers->m_highrank_exp);
		hook_manager::add<game_hooks::infinite_gathering>("Inf Gathering", g_pointers->m_inf_gathering);
		hook_manager::add<game_hooks::research_experience>("Research EXP Mult", g_pointers->m_research_exp);
		hook_manager::add<game_hooks::highlighted_armour>("Equipment Hook", g_pointers->m_armor_ptr);
		hook_manager::add<player_atk_stat>("Atk Hook", g_pointers->m_atk_ptr);
		hook_manager::add<player_affinity_stat>("Aff Hook", g_pointers->m_affinity_ptr);
		hook_manager::add<game_hooks::deploy_character>("Character Hook", g_pointers->m_character_ptr);
		hook_manager::add<game_hooks::highlighted_weapon>("Weapon Hook", g_pointers->m_weapon_ptr);
		hook_manager::add<game_hooks::meal_handler>("Meal Handler Hook", g_pointers->m_meal_handler);
		hook_manager::add<game_hooks::palico_experience_handler>("Palico Exp Handler Hook", g_pointers->m_palico_experiece_handler);
		hook_manager::add<game_hooks::player_forward>("Player Coordinates Hook", g_pointers->m_player_forward);
		hook_manager::add<game_hooks::player_position>("Player Position Hook", g_pointers->m_player_position);
		hook_manager::add<game_hooks::waypoint>("Waypoint Position Hook", g_pointers->m_waypoint);
		hook_manager::add<game_hooks::monster_part_breaking>("Easy Part Break Hook", g_pointers->m_easy_breaker);
		hook_manager::add<game_hooks::monster_on_map>("Monster On Map Progress Hook", g_pointers->m_reveal_monster);
		hook_manager::add<game_hooks::bow_gun_effect>("Bow Gun Effect Hook", g_pointers->m_bow_gun_effect);

		// Weapon mid-hooks (Safi Full V2)
		if (g_pointers->m_gs_inst_chrgs)
			mid_hook::add<game_hooks::mid_gs_charge>("Instantly Charged GS", g_pointers->m_gs_inst_chrgs);
		if (g_pointers->m_spirit_gauge)
			mid_hook::add<game_hooks::mid_spirit_gauge>("Inf Spirit Gauge", g_pointers->m_spirit_gauge);
		if (g_pointers->m_demon_mode)
			mid_hook::add<game_hooks::mid_demon_mode>("Inf Demon Mode", g_pointers->m_demon_mode);
		if (g_pointers->m_fully_charged_switch_axe)
			mid_hook::add<game_hooks::mid_switch_axe>("Fully Charged Switch Axe", g_pointers->m_fully_charged_switch_axe);
		if (g_pointers->m_gl_ammo)
			mid_hook::add<game_hooks::mid_gl_ammo>("Inf GL Ammo/Wyrmstake", g_pointers->m_gl_ammo);
		if (g_pointers->m_gl_cooldown)
			mid_hook::add<game_hooks::mid_gl_cooldown>("No GL Cooldown", g_pointers->m_gl_cooldown);
		if (g_pointers->m_charged_blade_phials)
			mid_hook::add<game_hooks::mid_cb_phials>("Inf CB Phials", g_pointers->m_charged_blade_phials);
		if (g_pointers->m_cb_sword)
			mid_hook::add<game_hooks::mid_cb_sword>("Inf CB Sword Charge", g_pointers->m_cb_sword);
		if (g_pointers->m_cb_shield)
			mid_hook::add<game_hooks::mid_cb_shield>("Inf CB Shield Charge", g_pointers->m_cb_shield);
		if (g_pointers->m_ig_extract)
			mid_hook::add<game_hooks::mid_ig_extract>("Inf Extract Duration", g_pointers->m_ig_extract);
		if (g_pointers->m_ig_stamina)
			mid_hook::add<game_hooks::mid_ig_stamina>("Inf IG Stamina", g_pointers->m_ig_stamina);
		if (g_pointers->m_ig_jump)
			mid_hook::add<game_hooks::mid_ig_jump>("Inf IG Jumping Attack", g_pointers->m_ig_jump);
		if (g_pointers->m_bow_coatings)
			mid_hook::add<game_hooks::mid_bow_coatings>("Bows Support All Coatings", g_pointers->m_bow_coatings);
		if (g_pointers->m_bow_gun_charge)
			mid_hook::add<game_hooks::mid_bow_charge>("Max Bow Charge", g_pointers->m_bow_gun_charge);
		if (g_pointers->m_bowgun_no_reload)
			mid_hook::add<game_hooks::mid_bowgun_no_reload>("Bowgun No Reload", g_pointers->m_bowgun_no_reload);
		if (g_pointers->m_inf_wyvern_blast)
			mid_hook::add<game_hooks::mid_wyvernblast>("Inf Wyvernblast", g_pointers->m_inf_wyvern_blast);
		if (g_pointers->m_wyvernheart)
			mid_hook::add<game_hooks::mid_wyvernheart>("Inf Wyvernheart", g_pointers->m_wyvernheart);
		if (g_pointers->m_wyvernheart_rapidfire)
			mid_hook::add<game_hooks::mid_wyvernheart_rapidfire>("Wyvernheart Rapidfire", g_pointers->m_wyvernheart_rapidfire);
		if (g_pointers->m_bowgun_no_recoil)
			mid_hook::add<game_hooks::mid_bowgun_no_recoil>("Bowgun No Recoil", g_pointers->m_bowgun_no_recoil);

		// Player & Misc mid-hooks
		if (g_pointers->m_part_breaker)
			mid_hook::add<game_hooks::mid_part_breaker>("Part Breaker", g_pointers->m_part_breaker);
		if (g_pointers->m_mount_reset)
			mid_hook::add<game_hooks::mid_mount_reset>("Mount Reset", g_pointers->m_mount_reset);
		if (g_pointers->m_frostcraft_drawn)
			mid_hook::add<game_hooks::mid_frostcraft_drawn>("Frostcraft Drawn", g_pointers->m_frostcraft_drawn);
		if (g_pointers->m_frostcraft_heat)
			mid_hook::add<game_hooks::mid_frostcraft_heat>("Frostcraft Heat", g_pointers->m_frostcraft_heat);
		if (g_pointers->m_lucky_voucher)
			mid_hook::add<game_hooks::mid_lucky_voucher>("Lucky Voucher", g_pointers->m_lucky_voucher);
		if (g_pointers->m_loaded_cannon)
			mid_hook::add<game_hooks::mid_load_cannon>("Loaded Cannon", g_pointers->m_loaded_cannon);
		if (g_pointers->m_pet_mgr)
			mid_hook::add<game_hooks::mid_pet_mgr>("Pet Manager", g_pointers->m_pet_mgr);
		if (g_pointers->m_pet_unlocker)
			mid_hook::add<game_hooks::mid_pet_unlocker>("Pet Unlocker", g_pointers->m_pet_unlocker);
		if (g_pointers->m_palico_gadget_cd)
			mid_hook::add<game_hooks::mid_palico_gadget_cd>("Palico Gadget CD", g_pointers->m_palico_gadget_cd);
		if (g_pointers->m_slinger_ammo)
			mid_hook::add<game_hooks::mid_slinger_ammo>("Slinger Ammo", g_pointers->m_slinger_ammo);
		if (g_pointers->m_maxed_effect_set)
			mid_hook::add<game_hooks::mid_maxed_effect_set>("Maxed Set Effect", g_pointers->m_maxed_effect_set);
		if (g_pointers->m_maxed_attribute)
			mid_hook::add<game_hooks::mid_maxed_attribute>("Maxed Armor Attribute", g_pointers->m_maxed_attribute);
		if (g_pointers->m_max_sharpness)
			mid_hook::add<game_hooks::mid_max_sharpness>("Max Sharpness", g_pointers->m_max_sharpness);
		if (g_pointers->m_max_slot_armor)
			mid_hook::add<game_hooks::mid_max_slot_armor>("Max Slot Armor", g_pointers->m_max_slot_armor);
		if (g_pointers->m_max_slot_armor_ranged)
			mid_hook::add<game_hooks::mid_max_slot_armor_ranged>("Max Slot Armor Ranged", g_pointers->m_max_slot_armor_ranged);
		if (g_pointers->m_max_slot_armor_melee)
			mid_hook::add<game_hooks::mid_max_slot_armor_melee>("Max Slot Armor Melee", g_pointers->m_max_slot_armor_melee);
		if (g_pointers->m_scoutfly_max)
			mid_hook::add<game_hooks::mid_scoutfly_max>("Scoutfly Max", g_pointers->m_scoutfly_max);
		if (g_pointers->m_scoutfly_level)
			mid_hook::add<game_hooks::mid_scoutfly_level>("Scoutfly Level", g_pointers->m_scoutfly_level);

		
	}
}
