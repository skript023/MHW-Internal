#pragma once
#include "core/commands/bool_command.hpp"
#include "game/settings.hpp"

namespace big::features
{
	class setting_bool_command : public bool_command
	{
		bool* m_setting_ptr;

	public:
		setting_bool_command(std::string name, std::string label, std::string description, bool* setting_ptr) :
			bool_command(std::move(name), std::move(label), std::move(description), setting_ptr ? *setting_ptr : false),
			m_setting_ptr(setting_ptr)
		{
		}

		virtual void on_call() override
		{
			bool_command::on_call();
			if (m_setting_ptr)
				*m_setting_ptr = get_state();
		}

		virtual void on_disable() override
		{
			if (m_setting_ptr)
				*m_setting_ptr = false;
		}
	};

	// Player Commands
	inline setting_bool_command _invulnerable("invulnerable", "Invulnerable", "Immune to all monster attacks and damage.", &g_settings.player.invulnerable);
	inline setting_bool_command _inf_stamina("inf_stamina", "Infinite Stamina", "Stamina never depletes.", &g_settings.player.inf_stamina);
	inline setting_bool_command _part_breaker("part_breaker", "Part Breaker", "Break monster parts quickly.", &g_settings.player.part_breaker);
	inline setting_bool_command _easy_part_breaker("easy_part_breaker", "Easy Part Breaker", "Monster parts break in one or few hits.", &g_settings.player.easy_part_breaker);
	inline setting_bool_command _inf_mount("inf_mount", "Infinite Mounting", "Never get thrown off while mounting monsters.", &g_settings.player.inf_mount);
	inline setting_bool_command _mount_reset("mount_reset", "Reset Mounting", "Instantly reset monster mount resistance.", &g_settings.player.mount_reset);
	inline setting_bool_command _inf_item("inf_item", "Infinite Item Use", "Consumables and items are not depleted when used.", &g_settings.player.inf_item);
	inline setting_bool_command _reveal_monster("reveal_monster", "Reveal Monsters", "Show all monsters on the mini-map without tracking.", &g_settings.player.reveal_monster);
	inline setting_bool_command _inf_gathering("inf_gathering", "Infinite Gathering", "Gather unlimited times from mining outcrops, plants and bones.", &g_settings.player.inf_gathering);
	inline setting_bool_command _palico_gadget_cd("palico_gadget_cd", "No Palico Gadget CD", "Palico gadgets can be used with zero cooldown.", &g_settings.player.palico_gadget_cd);
	inline setting_bool_command _inf_lucky_voucher("inf_lucky_voucher", "Infinite Lucky Voucher", "Always have lucky vouchers available.", &g_settings.player.inf_lucky_voucher);
	inline setting_bool_command _cannon_always_loaded("cannon_always_loaded", "Cannon Always Loaded", "Cannons and ballistas are permanently loaded.", &g_settings.player.cannon_always_loaded);
	inline setting_bool_command _inf_cannon_fire("inf_cannon_fire", "Infinite Cannon Fire", "Cannons fire without needing reload.", &g_settings.player.inf_cannon_fire);
	inline setting_bool_command _unlock_all_pet("unlock_all_pet", "Unlock All Pets", "Unlock all endemic life pets.", &g_settings.player.unlock_all_pet);
	inline setting_bool_command _scoutfly_max_level("scoutfly_max_level", "Max Scoutfly Level", "Max out scoutfly tracking level immediately.", &g_settings.player.scoutfly_max_level);

	// Weapon Commands
	inline setting_bool_command _max_sharpness("max_sharpness", "Max Sharpness", "Melee weapon sharpness never degrades.", &g_settings.player.max_sharpness);
	inline setting_bool_command _maxed_stat_set("maxed_stat_set", "Maxed Stat Set", "Max out equipment stats on hover or inspect.", &g_settings.player.maxed_stat_set);
	inline setting_bool_command _maxed_armor_effect("maxed_armor_effect", "Maxed Armor Effect", "Activate max level skills on equipped armor.", &g_settings.player.maxed_armor_effect);
	inline setting_bool_command _max_slot_armor("max_slot_armor", "Max Slot Armor", "Maximum decoration slots on all armor pieces.", &g_settings.weapon.max_slot_armor);
	inline setting_bool_command _inf_slinger_ammo("inf_slinger_ammo", "Infinite Slinger Ammo", "Slinger ammo is not consumed.", &g_settings.weapon.inf_slinger_ammo);

	// Frostcraft Commands
	inline setting_bool_command _inf_frostcraft("inf_frostcraft", "Infinite Frostcraft", "Frostcraft gauge stays permanently full.", &g_settings.weapon.inf_frostcraft);
	inline setting_bool_command _frostcraft_recharge("frostcraft_recharge", "Frostcraft Recharge", "Accelerate Frostcraft gauge replenishment.", &g_settings.weapon.frostcraft_recharge);

	// Safi Full V2 Weapon Buff Commands
	inline setting_bool_command _inst_chrgs_gs("inst_chrgs_gs", "Instantly Charged GS", "Great Sword charges to max level immediately.", &g_settings.weapon.inst_chrgs_gs);
	inline setting_bool_command _inf_spirit_gauge("inf_spirit_gauge", "Infinite Spirit Gauge", "Long Sword spirit gauge remains full and red.", &g_settings.weapon.inf_spirit_gauge);
	inline setting_bool_command _inf_demon_mode("inf_demon_mode", "Infinite Demon Mode", "Dual Blades demon mode stamina never depletes.", &g_settings.weapon.inf_demon_mode);
	inline setting_bool_command _full_charged_switch_axe("full_charged_switch_axe", "Fully Charged Switch Axe", "Switch Axe sword mode always amped.", &g_settings.weapon.full_charged_switch_axe);
	inline setting_bool_command _inf_gl_ammo("inf_gl_ammo", "Infinite GL Ammo / Stake", "Gunlance shells and wyrmstake never run out.", &g_settings.weapon.inf_gl_ammo);
	inline setting_bool_command _no_gl_cooldown("no_gl_cooldown", "No Gunlance Cooldown", "Zero cooldown on wyvern's fire.", &g_settings.weapon.no_gl_cooldown);
	inline setting_bool_command _cb_phials("cb_phials", "Infinite CB Phials", "Charge Blade phials always 6 and shield white.", &g_settings.weapon.cb_phials);
	inline setting_bool_command _cb_sword_charge("cb_sword_charge", "Infinite CB Sword Charge", "Infinite duration for Charge Blade sword charge buff.", &g_settings.weapon.cb_sword_charge);
	inline setting_bool_command _cb_shield_charge("cb_shield_charge", "Infinite CB Shield Charge", "Infinite duration for Charge Blade shield charge buff.", &g_settings.weapon.cb_shield_charge);
	inline setting_bool_command _ig_extract("ig_extract", "Infinite IG Extract Duration", "Triple buff extract duration permanently extended.", &g_settings.weapon.ig_extract);
	inline setting_bool_command _ig_stamina("ig_stamina", "Infinite Kinsect Stamina", "Insect Glaive kinsect stamina never runs out.", &g_settings.weapon.ig_stamina);
	inline setting_bool_command _ig_jump("ig_jump", "Infinite IG Vaulting", "Vault and air dash indefinitely with Insect Glaive.", &g_settings.weapon.ig_jump);
	inline setting_bool_command _bow_coatings("bow_coatings", "Bows Support All Coatings", "All bows can use every arrow coating.", &g_settings.weapon.bow_coatings);
	inline setting_bool_command _bow_charge("bow_charge", "Max Bow Charge", "Bow attacks instantly reach max charge level.", &g_settings.weapon.bow_charge);
	inline setting_bool_command _bowgun_no_reload("bowgun_no_reload", "Bowgun No Reload", "Bowguns fire continuously without needing to reload.", &g_settings.weapon.bowgun_no_reload);
	inline setting_bool_command _inf_wyvern_blast("inf_wyvern_blast", "Infinite Wyvernblast", "Light Bowgun wyvernblast mines never deplete.", &g_settings.weapon.inf_wyvern_blast);
	inline setting_bool_command _inf_wyvernheart("inf_wyvernheart", "Infinite Wyvernheart", "HBG wyvernheart special ammo gauge never drains.", &g_settings.weapon.inf_wyvernheart);
	inline setting_bool_command _wyvernheart_rapidfire("wyvernheart_rapidfire", "Wyvernheart Rapidfire", "Fire wyvernheart at extreme rapid rate with zero delay.", &g_settings.weapon.wyvernheart_rapidfire);
	inline setting_bool_command _bowgun_no_recoil("bowgun_no_recoil", "Bowgun No Recoil & Full Mods", "Eliminate bowgun recoil and apply full mods automatically.", &g_settings.weapon.bowgun_no_recoil);

	// Crafting Commands
	inline setting_bool_command _ignore_material("ignore_material", "Ignore Material", "Forge and upgrade equipment without consuming materials or zenny.", &g_settings.crafting.ignore_material);
	inline setting_bool_command _unlock_equipment("unlock_equipment", "Unlock Equipment", "Display and forge all weapons and armor regardless of hunt progression.", &g_settings.crafting.unlock_equipment);
}
