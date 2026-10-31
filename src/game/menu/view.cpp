#include "game/features/features.hpp"
#include "view.hpp"
#include "core/utility/fnv1a.hpp"
#include "player_menu.h"
#include "teleport_menu.h"
#include "game/mhw/character/character_service.hpp"
#include "game/mhw/meal/meal_service.hpp"
#include "game/mhw/meal/meal_data.hpp"
#include "game/mhw/equipment/weapon_service.hpp"
#include "game/mhw/equipment/armour_service.hpp"
#include "game/mhw/qol/qol_service.hpp"
#include <astra/host/notification.hpp>
#include <vector>
#include <mutex>

namespace big
{
	static const std::vector<const char*> s_meal_buff_names = [] {
		std::vector<const char*> names;
		names.reserve(meal_data::meal_buff.size());
		for (const auto& b : meal_data::meal_buff)
		{
			names.push_back(b.second.data());
		}
		return names;
	}();

	void view::register_submenu()
	{
		player_submenu();
		weapon_submenu();
		crafting_submenu();
		teleport_submenu();
		qol_submenu();
		setting_submenu();
	}

	void view::player_submenu()
	{
		canvas::add_tab<regular_submenu>("Player", SubmenuPlayer, [](regular_submenu* sub) {
			sub->add_option<bool_option<>>("invulnerable"_hash);
			sub->add_option<bool_option<>>("inf_stamina"_hash);
			sub->add_option<bool_option<>>("part_breaker"_hash);
			sub->add_option<bool_option<>>("easy_part_breaker"_hash);
			sub->add_option<bool_option<>>("inf_mount"_hash);
			sub->add_option<bool_option<>>("mount_reset"_hash);
			sub->add_option<bool_option<>>("inf_item"_hash);
			sub->add_option<bool_option<>>("reveal_monster"_hash);
			sub->add_option<bool_option<>>("inf_gathering"_hash);
			sub->add_option<bool_option<>>("palico_gadget_cd"_hash);
			sub->add_option<bool_option<>>("inf_lucky_voucher"_hash);
			sub->add_option<bool_option<>>("cannon_always_loaded"_hash);
			sub->add_option<bool_option<>>("inf_cannon_fire"_hash);
			sub->add_option<bool_option<>>("unlock_all_pet"_hash);
			sub->add_option<bool_option<>>("scoutfly_max_level"_hash);
			sub->add_option<sub_option>("Player Stats & EXP", "Multipliers for rank experience and combat stats.", "SubmenuStats"_hash);
			sub->add_option<sub_option>("Meal Editor", "Customize Canteen food skills and stat buffs.", "SubmenuMeal"_hash);
		});

		canvas::add_submenu<regular_submenu>("Player Stats & EXP", "SubmenuStats"_hash, [](regular_submenu* sub) {
			sub->add_option<number_option<float>>("Critical Boost", "Critical damage multiplier.", &features::_critical_boost.get_state(), 1.f, 100.f, 0.25f, 2);
			sub->add_option<number_option<float>>("Attack Bonus", "Raw attack bonus added by hook.", &g_atk_addr, 0.f, 10000.f, 10.f, 1);
			sub->add_option<number_option<int>>("Defense Bonus", "Defense bonus added by hook.", &g_def_addr, 0, 100000, 100);
			sub->add_option<number_option<float>>("Damage Received %", "Damage received multiplier applied by defense hook.", &g_def_rate_addr, 0.f, 10.f, 0.05f, 2);
			sub->add_option<number_option<float>>("Affinity Bonus %", "Affinity percentage added by hook.", &g_affinity_addr, -100.f, 500.f, 10.f, 1);
			sub->add_option<number_option<float>>("Research EXP Mult", "Multiplier for monster research points.", &features::_research_exp.get_state(), 1.f, 50.f, 1.f, 1);
			sub->add_option<bool_option<>>("research_exp_mult"_hash);
			sub->add_option<number_option<int>>("mr_ex_mult"_hash, 1);
			sub->add_option<number_option<int>>("hr_ex_mult"_hash, 1);
			sub->add_option<number_option<int>>("palico_exp_mult"_hash, 1);

			sub->add_option<reguler_option>("Add 1,000,000 Zeni", "Add 1,000,000 Zeni to hunter balance.", [] {
				if (true)
				{
					character_service::set_zeni(character_service::get_zeni() + 1000000);
					notification::success("Economy", "Added 1,000,000 Zeni!");
				}
			});

			sub->add_option<reguler_option>("Add 100,000 Research Pts", "Add 100,000 Research Points to hunter balance.", [] {
				if (true)
				{
					character_service::set_research_points(character_service::get_research_points() + 100000);
					notification::success("Economy", "Added 100,000 Research Points!");
				}
			});
		});

		canvas::add_submenu<regular_submenu>("Meal Editor", "SubmenuMeal"_hash, [](regular_submenu* sub) {
			auto meal = meal_service::get_stats();
			if (!meal)
			{
				sub->add_option<reguler_option>("No Meal Active", "Visit the Canteen / eat a meal first to edit stats.", [] {});
				return;
			}

			sub->add_option<reguler_option>("Max All Meal Buffs", "Max out Attack, Defense, Health, Stamina and Fresh Ingredients.", [meal] {
				meal->m_attack_effect = 1000;
				meal->m_defense_effect = 1000;
				meal->m_health_effect = 1000;
				meal->m_stamina_effect = 1000;
				meal->m_palico_health = 1000;
				meal->m_fresh_ingredients = 1000;
				notification::success("Meal Editor", "Maximized all meal buffs!");
			});

			sub->add_option<number_option<int32_t>>("Attack Up", "Attack effect bonus from meal.", &meal->m_attack_effect, 0, 1000, 10);
			sub->add_option<number_option<int32_t>>("Defense Up", "Defense effect bonus from meal.", &meal->m_defense_effect, 0, 1000, 10);
			sub->add_option<number_option<int32_t>>("Health Up", "Health effect bonus from meal.", &meal->m_health_effect, 0, 1000, 10);
			sub->add_option<number_option<int32_t>>("Stamina Up", "Stamina effect bonus from meal.", &meal->m_stamina_effect, 0, 1000, 10);
			sub->add_option<number_option<int32_t>>("Palico Health Up", "Palico health bonus from meal.", &meal->m_palico_health, 0, 1000, 10);
			sub->add_option<number_option<int32_t>>("Fresh Ingredients", "Fresh ingredient activation chance.", &meal->m_fresh_ingredients, 0, 1000, 10);

			sub->add_option<sub_option>("Meal Skills", "Select food skills for Buff 1, 2, and 3.", "SubmenuMealSkills"_hash);
		});

		canvas::add_submenu<regular_submenu>("Meal Skills", "SubmenuMealSkills"_hash, [](regular_submenu* sub) {
			auto meal = meal_service::get_stats();
			if (!meal)
			{
				sub->add_option<reguler_option>("No Meal Active", "Visit the Canteen / eat a meal first.", [] {});
				return;
			}

			if (meal->m_skill_a < 0 || meal->m_skill_a >= static_cast<int32_t>(s_meal_buff_names.size()))
				meal->m_skill_a = 0;
			if (meal->m_skill_b < 0 || meal->m_skill_b >= static_cast<int32_t>(s_meal_buff_names.size()))
				meal->m_skill_b = 0;
			if (meal->m_skill_c < 0 || meal->m_skill_c >= static_cast<int32_t>(s_meal_buff_names.size()))
				meal->m_skill_c = 0;

			sub->add_option<choose_option<const char*, int32_t>>("Meal Skill 1", "Select first food skill.", &s_meal_buff_names, &meal->m_skill_a);
			sub->add_option<choose_option<const char*, int32_t>>("Meal Skill 2", "Select second food skill.", &s_meal_buff_names, &meal->m_skill_b);
			sub->add_option<choose_option<const char*, int32_t>>("Meal Skill 3", "Select third food skill.", &s_meal_buff_names, &meal->m_skill_c);
		});
	}

	void view::weapon_submenu()
	{
		canvas::add_tab<regular_submenu>("Weapon", SubmenuWeapon, [](regular_submenu* sub) {
			sub->add_option<bool_option<>>("max_sharpness"_hash);
			sub->add_option<bool_option<>>("maxed_stat_set"_hash);
			sub->add_option<bool_option<>>("maxed_armor_effect"_hash);
			sub->add_option<bool_option<>>("max_slot_armor"_hash);
			sub->add_option<bool_option<>>("inf_slinger_ammo"_hash);
			sub->add_option<sub_option>("Frostcraft", "Frostcraft gauge settings and multipliers.", "SubmenuFrostcraft"_hash);
			sub->add_option<sub_option>("Weapon Buffs", "Specific weapon mechanics and instant charge buffs.", "SubmenuWeaponBuffs"_hash);
			sub->add_option<sub_option>("Weapon Editor", "Inspect or edit currently hovered melee weapon stats.", "SubmenuWeaponEditor"_hash);
			sub->add_option<sub_option>("Armour Editor", "Inspect or edit currently hovered armour piece.", "SubmenuArmourEditor"_hash);
		});

		canvas::add_submenu<regular_submenu>("Frostcraft", "SubmenuFrostcraft"_hash, [](regular_submenu* sub) {
			sub->add_option<bool_option<>>("inf_frostcraft"_hash);
			sub->add_option<bool_option<>>("frostcraft_recharge"_hash);
			sub->add_option<number_option<float>>("Frostcraft Drawn Mult", "Damage boost when weapon is drawn.", &features::_frostcraft_drawn.get_state(), 1.f, 20.f, 0.5f, 1);
			sub->add_option<number_option<float>>("Frostcraft Heat", "Frostcraft heat rate parameter.", &features::_frostcraft_heat.get_state(), 1.f, 100.f, 5.f, 1);
		});

		canvas::add_submenu<regular_submenu>("Weapon Buffs", "SubmenuWeaponBuffs"_hash, [](regular_submenu* sub) {
			sub->add_option<bool_option<>>("inst_chrgs_gs"_hash);
			sub->add_option<bool_option<>>("inf_spirit_gauge"_hash);
			sub->add_option<bool_option<>>("inf_demon_mode"_hash);
			sub->add_option<bool_option<>>("full_charged_switch_axe"_hash);
			sub->add_option<bool_option<>>("inf_gl_ammo"_hash);
			sub->add_option<bool_option<>>("no_gl_cooldown"_hash);
			sub->add_option<bool_option<>>("cb_phials"_hash);
			sub->add_option<bool_option<>>("cb_sword_charge"_hash);
			sub->add_option<bool_option<>>("cb_shield_charge"_hash);
			sub->add_option<bool_option<>>("ig_extract"_hash);
			sub->add_option<bool_option<>>("ig_stamina"_hash);
			sub->add_option<bool_option<>>("ig_jump"_hash);
			sub->add_option<bool_option<>>("bow_coatings"_hash);
			sub->add_option<bool_option<>>("bow_charge"_hash);
			sub->add_option<bool_option<>>("bowgun_no_reload"_hash);
			sub->add_option<bool_option<>>("inf_wyvern_blast"_hash);
			sub->add_option<bool_option<>>("inf_wyvernheart"_hash);
			sub->add_option<bool_option<>>("wyvernheart_rapidfire"_hash);
			sub->add_option<bool_option<>>("bowgun_no_recoil"_hash);
		});

		canvas::add_submenu<regular_submenu>("Weapon Editor", "SubmenuWeaponEditor"_hash, [](regular_submenu* sub) {
			auto melee = weapon_service::get_melee();
			if (!melee)
			{
				sub->add_option<reguler_option>("No Weapon Selected", "Hover over a melee weapon in Change Equipment box.", [] {});
				return;
			}

			sub->add_option<number_option<unsigned short>>("Attack", "Base weapon raw attack power.", &melee->m_attack, (unsigned short)0, (unsigned short)10000, (unsigned short)50);
			sub->add_option<number_option<unsigned short>>("Defense", "Weapon defense bonus.", &melee->m_defense, (unsigned short)0, (unsigned short)5000, (unsigned short)10);
			sub->add_option<number_option<uint8_t>>("Affinity %", "Base affinity percentage.", &melee->m_affinity, (uint8_t)0, (uint8_t)100, (uint8_t)5);
			sub->add_option<number_option<uint8_t>>("Sharpness Quality", "Quality level of sharpness gauge.", &melee->m_sharpness_quality, (uint8_t)0, (uint8_t)10, (uint8_t)1);
			sub->add_option<number_option<uint8_t>>("Sharpness Amount", "Maximum sharpness amount.", &melee->m_sharpness_amount, (uint8_t)0, (uint8_t)255, (uint8_t)10);
			sub->add_option<number_option<uint8_t>>("Rarity (-1)", "Weapon rarity level minus 1.", &melee->m_rarity, (uint8_t)0, (uint8_t)11, (uint8_t)1);
			sub->add_option<number_option<unsigned short>>("Elemental Atk", "Elemental attack rating.", &melee->m_elemental_atk, (unsigned short)0, (unsigned short)10000, (unsigned short)50);
			sub->add_option<number_option<unsigned short>>("Hidden Elem Atk", "Hidden element attack rating.", &melee->m_hidden_element_atk, (unsigned short)0, (unsigned short)10000, (unsigned short)50);
		});

		canvas::add_submenu<regular_submenu>("Armour Editor", "SubmenuArmourEditor"_hash, [](regular_submenu* sub) {
			auto armor = armour_service::get();
			if (!armor)
			{
				sub->add_option<reguler_option>("No Armour Selected", "Hover over an armour piece in Change Equipment box.", [] {});
				return;
			}

			sub->add_option<reguler_option>("Max Defense & Slots", "Set defense to 1000 and all 3 deco slots to lvl 4.", [armor] {
				armor->m_defense = 1000;
				armor->m_slot_a_level = 4;
				armor->m_slot_b_level = 4;
				armor->m_slot_c_level = 4;
				armor->m_fire_res = 50;
				armor->m_water_res = 50;
				armor->m_thunder_res = 50;
				armor->m_ice_res = 50;
				armor->m_dragon_res = 50;
				notification::success("Armour Editor", "Maximized armour piece!");
			});

			sub->add_option<number_option<short>>("Defense", "Base armor defense rating.", &armor->m_defense, (short)0, (short)10000, (short)20);
			sub->add_option<number_option<int8_t>>("Slot 1 Level", "Decoration slot 1 level (0-4).", &armor->m_slot_a_level, (int8_t)0, (int8_t)4, (int8_t)1);
			sub->add_option<number_option<int8_t>>("Slot 2 Level", "Decoration slot 2 level (0-4).", &armor->m_slot_b_level, (int8_t)0, (int8_t)4, (int8_t)1);
			sub->add_option<number_option<int8_t>>("Slot 3 Level", "Decoration slot 3 level (0-4).", &armor->m_slot_c_level, (int8_t)0, (int8_t)4, (int8_t)1);
			sub->add_option<number_option<int8_t>>("Fire Resistance", "Fire elemental resistance.", &armor->m_fire_res, (int8_t)-50, (int8_t)100, (int8_t)5);
			sub->add_option<number_option<int8_t>>("Water Resistance", "Water elemental resistance.", &armor->m_water_res, (int8_t)-50, (int8_t)100, (int8_t)5);
			sub->add_option<number_option<int8_t>>("Thunder Resistance", "Thunder elemental resistance.", &armor->m_thunder_res, (int8_t)-50, (int8_t)100, (int8_t)5);
			sub->add_option<number_option<int8_t>>("Ice Resistance", "Ice elemental resistance.", &armor->m_ice_res, (int8_t)-50, (int8_t)100, (int8_t)5);
			sub->add_option<number_option<int8_t>>("Dragon Resistance", "Dragon elemental resistance.", &armor->m_dragon_res, (int8_t)-50, (int8_t)100, (int8_t)5);
		});
	}

	void view::crafting_submenu()
	{
		canvas::add_tab<regular_submenu>("Crafting", SubmenuCrafting, [](regular_submenu* sub) {
			sub->add_option<bool_option<>>("ignore_material"_hash);
			sub->add_option<bool_option<>>("unlock_equipment"_hash);
		});
	}

	void view::teleport_submenu()
	{
		canvas::add_tab<regular_submenu>("Teleport", SubmenuTeleport, [](regular_submenu* sub) {
			sub->add_option<reguler_option>("Teleport to Waypoint", "Instantly teleport hunter to map waypoint pin.", [] {
				if (true)
				{
					character_service::set_player_to_waypoint();
					notification::info("Teleport", "Teleported to waypoint.");
				}
			});

			sub->add_option<reguler_option>("Teleport Forward", "Teleport in the direction the hunter is facing.", [] {
				if (true)
				{
					character_service::set_player_forward();
				}
			});

			sub->add_option<reguler_option>("Save Current Location", "Save current hunter position to config file.", [] {
				if (true)
				{
					auto pos = character_service::get_position();
					auto list = teleport_menu::location_list();
					std::string name = "Location " + std::to_string(list.size() + 1);
					teleport_menu::save(pos, name);
					notification::success("Teleport", ("Saved as " + name).c_str());
				}
			});

			sub->add_option<sub_option>("Saved Locations", "Teleport to previously saved custom coordinates.", "SubmenuTeleportSaved"_hash);
		});

		canvas::add_submenu<regular_submenu>("Saved Locations", "SubmenuTeleportSaved"_hash, [](regular_submenu* sub) {
			auto list = teleport_menu::location_list();
			if (list.empty())
			{
				sub->add_option<reguler_option>("No Saved Locations", "Use 'Save Current Location' to save positions.", [] {});
				return;
			}

			for (const auto& name : list)
			{
				sub->add_option<reguler_option>(name.c_str(), "Press Enter to teleport to this location.", [name] {
					teleport_menu::load_location(name);
					notification::info("Teleport", ("Teleported to " + name).c_str());
				});
			}

			sub->add_option<reguler_option>("Clear All Saved", "Delete all saved locations.", [] {
				auto list = teleport_menu::location_list();
				for (const auto& name : list)
				{
					teleport_menu::delete_location(name);
				}
				notification::info("Teleport", "Cleared all saved locations.");
			});
		});
	}

	void view::qol_submenu()
	{
		canvas::add_tab<regular_submenu>("QoL & Flow", SubmenuQoL, [](regular_submenu* sub) {
			sub->add_option<bool_option<>>("skippable_cutscenes"_hash);
			sub->add_option<reguler_option>("Skip Current Cutscene", "Force skip active cutscene immediately.", [] {
				qol_service::skip_cutscene();
			});

			sub->add_option<bool_option<>>("post_quest_timer_skip"_hash);
			sub->add_option<bool_option<>>("auto_post_quest_timer_skip"_hash);
			sub->add_option<reguler_option>("Skip Quest Timer Now", "Instantly end post-quest wait timer (Hotkeys: 'Y' or Menu).", [] {
				qol_service::skip_post_quest_timer();
			});

			sub->add_option<bool_option<>>("instant_gathering"_hash);
			sub->add_option<bool_option<>>("zero_delay_carving"_hash);
			sub->add_option<number_option<float>>("Carve Speed Mult", "Carving animation speed multiplier.", &features::_carve_speed_multiplier.get_state(), 1.f, 25.f, 1.f, 1);

			sub->add_option<bool_option<>>("full_hub_sprint"_hash);
			sub->add_option<number_option<float>>("Hub Speed Mult", "Hub movement speed multiplier.", &features::_hub_speed_multiplier.get_state(), 1.f, 5.f, 0.25f, 2);

			sub->add_option<bool_option<>>("remote_camp_facilities"_hash);
			sub->add_option<reguler_option>("Open Remote Item Box", "Open Item Box anywhere in the field (Hotkey: F7).", [] {
				qol_service::open_remote_item_box();
			});
			sub->add_option<reguler_option>("Open Remote Equipment", "Open Change Equipment / Loadouts anywhere (Hotkey: F8).", [] {
				qol_service::open_remote_equipment();
			});

			sub->add_option<bool_option<>>("quick_quest_restart"_hash);
			sub->add_option<reguler_option>("Quick Quest Restart", "Restart active quest from 00:00 (Hotkey: Ctrl+R).", [] {
				qol_service::quick_restart_quest();
			});
			sub->add_option<reguler_option>("Quick Quest Reset", "Cleanly abandon quest back to base camp (Hotkey: Alt+R).", [] {
				qol_service::quick_reset_quest();
			});

			sub->add_option<bool_option<>>("tic_rate_fix"_hash);
		});
	}

	void view::setting_submenu()
	{
		canvas::add_settings_tab(SubmenuSettings, SubmenuSettingsInput, [](regular_submenu* sub) {
			sub->add_option<bool_option<>>("overlay"_hash);
			sub->add_option<bool_option<>>("overlay_monster"_hash);
			sub->add_option<bool_option<>>("overlay_crown"_hash);
			sub->add_option<bool_option<>>("overlay_enrage_bar"_hash);
			sub->add_option<bool_option<>>("overlay_monster_weakness"_hash);
			sub->add_option<bool_option<>>("overlay_hitzone_values"_hash);
			sub->add_option<bool_option<>>("overlay_monster_parts"_hash);
			sub->add_option<bool_option<>>("overlay_monster_ailments"_hash);
			sub->add_option<bool_option<>>("overlay_party_dps"_hash);
			sub->add_option<bool_option<>>("overlay_dropped_materials"_hash);
			sub->add_option<bool_option<>>("light_pillars_shinies"_hash);
			sub->add_option<bool_option<>>("overlay_hunter_buffs"_hash);
			sub->add_option<bool_option<>>("overlay_team_esp"_hash);
			sub->add_option<bool_option<>>("overlay_hunter"_hash);
			sub->add_option<reguler_option>("Unload", "Safely unhook and eject the DLL.", [] {
				g_running = false;
			});
		});
	}
}
