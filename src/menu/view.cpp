#include "view.hpp"
#include "menu_settings.hpp"
#include "features/player.hpp"
#include "features/weapon.hpp"
#include "utility/fnv1a.hpp"
#include "player_menu.h"
#include "teleport_menu.h"
#include "services/character/character_service.hpp"
#include "services/meal/meal_service.hpp"
#include "services/meal/meal_data.hpp"
#include "services/equipment/weapon_service.hpp"
#include "services/equipment/armour_service.hpp"
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
		setting_submenu();
	}

	void view::player_submenu()
	{
		canvas::add_tab<regular_submenu>("Player", SubmenuPlayer, [](regular_submenu* sub) {
			sub->add_option<bool_option<bool>>("Invulnerable", "Immune to all monster attacks and damage.", &g_settings.player.invulnerable);
			sub->add_option<bool_option<bool>>("Infinite Stamina", "Stamina never depletes.", &g_settings.player.inf_stamina);
			sub->add_option<bool_option<bool>>("Part Breaker", "Break monster parts quickly.", &g_settings.player.part_breaker);
			sub->add_option<bool_option<bool>>("Easy Part Breaker", "Monster parts break in one or few hits.", &g_settings.player.easy_part_breaker);
			sub->add_option<bool_option<bool>>("Infinite Mounting", "Never get thrown off while mounting monsters.", &g_settings.player.inf_mount);
			sub->add_option<bool_option<bool>>("Reset Mounting", "Instantly reset monster mount resistance.", &g_settings.player.mount_reset);
			sub->add_option<bool_option<bool>>("Infinite Item Use", "Consumables and items are not depleted when used.", &g_settings.player.inf_item);
			sub->add_option<bool_option<bool>>("Reveal Monsters", "Show all monsters on the mini-map without tracking.", &g_settings.player.reveal_monster);
			sub->add_option<bool_option<bool>>("Infinite Gathering", "Gather unlimited times from mining outcrops, plants and bones.", &g_settings.player.inf_gathering);
			sub->add_option<bool_option<bool>>("No Palico Gadget CD", "Palico gadgets can be used with zero cooldown.", &g_settings.player.palico_gadget_cd);
			sub->add_option<bool_option<bool>>("Infinite Lucky Voucher", "Always have lucky vouchers available.", &g_settings.player.inf_lucky_voucher);
			sub->add_option<bool_option<bool>>("Cannon Always Loaded", "Cannons and ballistas are permanently loaded.", &g_settings.player.cannon_always_loaded);
			sub->add_option<bool_option<bool>>("Infinite Cannon Fire", "Cannons fire without needing reload.", &g_settings.player.inf_cannon_fire);
			sub->add_option<bool_option<bool>>("Unlock All Pets", "Unlock all endemic life pets.", &g_settings.player.unlock_all_pet);
			sub->add_option<bool_option<bool>>("Max Scoutfly Level", "Max out scoutfly tracking level immediately.", &g_settings.player.scoutfly_max_level);
			sub->add_option<sub_option>("Player Stats & EXP", "Multipliers for rank experience and combat stats.", "SubmenuStats"_hash);
			sub->add_option<sub_option>("Meal Editor", "Customize Canteen food skills and stat buffs.", "SubmenuMeal"_hash);
		});

		canvas::add_submenu<regular_submenu>("Player Stats & EXP", "SubmenuStats"_hash, [](regular_submenu* sub) {
			sub->add_option<number_option<float>>("Critical Boost", "Critical damage multiplier.", &g_settings.player.critical_boost, 1.f, 100.f, 0.25f, 2);
			sub->add_option<number_option<float>>("Attack Bonus", "Raw attack bonus added by hook.", &g_atk_addr, 0.f, 10000.f, 10.f, 1);
			sub->add_option<number_option<int>>("Defense Bonus", "Defense bonus added by hook.", &g_def_addr, 0, 100000, 100);
			sub->add_option<number_option<float>>("Damage Received %", "Damage received multiplier applied by defense hook.", &g_def_rate_addr, 0.f, 10.f, 0.05f, 2);
			sub->add_option<number_option<float>>("Affinity Bonus %", "Affinity percentage added by hook.", &g_affinity_addr, -100.f, 500.f, 10.f, 1);
			sub->add_option<number_option<float>>("Research EXP Mult", "Multiplier for monster research points.", &g_settings.player.research_exp, 1.f, 50.f, 1.f, 1);
			sub->add_option<number_option<int>>("MR EXP Mult", "Master Rank experience multiplier.", &g_settings.player.mr_ex_mult, 1, 100, 1);
			sub->add_option<number_option<int>>("HR EXP Mult", "High Rank experience multiplier.", &g_settings.player.hr_ex_mult, 1, 100, 1);
			sub->add_option<number_option<int>>("Palico EXP Mult", "Palico experience multiplier.", &g_settings.player.palico_exp_mult, 1, 100, 1);

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
			sub->add_option<bool_option<bool>>("Max Sharpness", "Melee weapon sharpness never degrades.", &g_settings.player.max_sharpness);
			sub->add_option<bool_option<bool>>("Maxed Stat Set", "Max out equipment stats on hover or inspect.", &g_settings.player.maxed_stat_set);
			sub->add_option<bool_option<bool>>("Maxed Armor Effect", "Activate max level skills on equipped armor.", &g_settings.player.maxed_armor_effect);
			sub->add_option<bool_option<bool>>("Max Slot Armor", "Maximum decoration slots on all armor pieces.", &g_settings.weapon.max_slot_armor);
			sub->add_option<bool_option<bool>>("Infinite Slinger Ammo", "Slinger ammo is not consumed.", &g_settings.weapon.inf_slinger_ammo);
			sub->add_option<sub_option>("Frostcraft", "Frostcraft gauge settings and multipliers.", "SubmenuFrostcraft"_hash);
			sub->add_option<sub_option>("Weapon Buffs", "Specific weapon mechanics and instant charge buffs.", "SubmenuWeaponBuffs"_hash);
			sub->add_option<sub_option>("Weapon Editor", "Inspect or edit currently hovered melee weapon stats.", "SubmenuWeaponEditor"_hash);
			sub->add_option<sub_option>("Armour Editor", "Inspect or edit currently hovered armour piece.", "SubmenuArmourEditor"_hash);
		});

		canvas::add_submenu<regular_submenu>("Frostcraft", "SubmenuFrostcraft"_hash, [](regular_submenu* sub) {
			sub->add_option<bool_option<bool>>("Infinite Frostcraft", "Frostcraft gauge stays permanently full.", &g_settings.weapon.inf_frostcraft);
			sub->add_option<bool_option<bool>>("Frostcraft Recharge", "Accelerate Frostcraft gauge replenishment.", &g_settings.weapon.frostcraft_recharge);
			sub->add_option<number_option<float>>("Frostcraft Drawn Mult", "Damage boost when weapon is drawn.", &g_settings.weapon.frostcraft_drawn, 1.f, 20.f, 0.5f, 1);
			sub->add_option<number_option<float>>("Frostcraft Heat", "Frostcraft heat rate parameter.", &g_settings.weapon.frostcraft_heat, 1.f, 100.f, 5.f, 1);
		});

		canvas::add_submenu<regular_submenu>("Weapon Buffs", "SubmenuWeaponBuffs"_hash, [](regular_submenu* sub) {
			sub->add_option<bool_option<bool>>("Instant GS Charges", "Great Sword charges to max level immediately.", &g_settings.weapon.inst_chrgs_gs);
			sub->add_option<bool_option<bool>>("Infinite Spirit Gauge", "Long Sword spirit gauge remains full.", &g_settings.weapon.inf_spirit_gauge);
			sub->add_option<bool_option<bool>>("Infinite Demon Mode", "Dual Blades demon mode stamina never depletes.", &g_settings.weapon.inf_demon_mode);
			sub->add_option<bool_option<bool>>("Fully Charged Switch Axe", "Switch Axe sword mode always amped.", &g_settings.weapon.full_charged_switch_axe);
			sub->add_option<bool_option<bool>>("Infinite GL Ammo / Stake", "Gunlance shells and wyrmstake never run out.", &g_settings.weapon.inf_gl_ammo);
			sub->add_option<bool_option<bool>>("No Gunlance Cooldown", "Zero cooldown on wyvern's fire.", &g_settings.weapon.no_gl_cooldown);
			sub->add_option<bool_option<bool>>("Infinite CB Phials", "Phials always maxed at 6 and shield charged white.", &g_settings.weapon.cb_phials);
			sub->add_option<bool_option<bool>>("Infinite CB Sword Charge", "Sword buff duration never expires.", &g_settings.weapon.cb_sword_charge);
			sub->add_option<bool_option<bool>>("Infinite CB Shield Charge", "Shield charge duration never expires.", &g_settings.weapon.cb_shield_charge);
			sub->add_option<bool_option<bool>>("Infinite IG Extract Duration", "Triple buff extract duration permanently active.", &g_settings.weapon.ig_extract);
			sub->add_option<bool_option<bool>>("Infinite Kinsect Stamina", "Kinsect stamina never depletes.", &g_settings.weapon.ig_stamina);
			sub->add_option<bool_option<bool>>("Infinite IG Vaulting", "Air vault and dash attacks indefinitely.", &g_settings.weapon.ig_jump);
			sub->add_option<bool_option<bool>>("Bows Support All Coatings", "All bows can use every coating type.", &g_settings.weapon.bow_coatings);
			sub->add_option<bool_option<bool>>("Max Bow Charge", "Bow attacks immediately release at max charge.", &g_settings.weapon.bow_charge);
			sub->add_option<bool_option<bool>>("Bowgun No Reload", "Bowguns fire continuously without reloading.", &g_settings.weapon.bowgun_no_reload);
			sub->add_option<bool_option<bool>>("Infinite Wyvernblast", "Light Bowgun wyvernblast mines recharge instantly.", &g_settings.weapon.inf_wyvern_blast);
			sub->add_option<bool_option<bool>>("Infinite Wyvernheart", "HBG wyvernheart special ammo never drains.", &g_settings.weapon.inf_wyvernheart);
			sub->add_option<bool_option<bool>>("Wyvernheart Rapidfire", "Fire wyvernheart at extreme rapidfire rate.", &g_settings.weapon.wyvernheart_rapidfire);
			sub->add_option<bool_option<bool>>("Bowgun No Recoil & Mods", "Eliminate recoil and automatically apply full bowgun mods.", &g_settings.weapon.bowgun_no_recoil);
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
			sub->add_option<bool_option<bool>>("Ignore Material", "Forge and upgrade equipment without consuming materials or zenny.", &g_settings.crafting.ignore_material);
			sub->add_option<bool_option<bool>>("Unlock Equipment", "Display and forge all weapons and armor regardless of hunt progression.", &g_settings.crafting.unlock_equipment);
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

	void view::setting_submenu()
	{
		canvas::add_settings_tab(SubmenuSettings, SubmenuSettingsInput, [](regular_submenu* sub) {
			sub->add_option<bool_option<bool>>("Show Overlay", "Display hunter, world, and monster info overlay on screen.", &g_settings.window.overlay);
			sub->add_option<bool_option<bool>>("Monster Health Bar", "Show monster health bar, size, and coordinates.", &g_settings.window.overlay_monster);
			sub->add_option<bool_option<bool>>("Crown Size Badges", "Show Mini Crown / Giant Gold Crown indicators.", &g_settings.window.overlay_crown);
			sub->add_option<bool_option<bool>>("Monster Enrage Bar", "Show monster enrage / state timer progress bar.", &g_settings.window.overlay_enrage_bar);
			sub->add_option<bool_option<bool>>("Hunter Info Panel", "Display hunter name, ranks, zeni, and world weather.", &g_settings.window.overlay_hunter);
			sub->add_option<reguler_option>("Unload", "Safely unhook and eject the DLL.", [] {
				g_running = false;
			});
		});
	}
}
