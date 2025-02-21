#pragma once
#include "logger.hpp"
#include "imgui.h"
#include "file_manager.hpp"

namespace big
{
	class settings;
	inline settings* g_settings{};

	class settings 
	{
		nlohmann::json default_options;
		nlohmann::json options;
	public:
		settings(file setting): settings_location(setting.get_path())
		{
			g_settings = this;
		}

		~settings()
		{
			g_settings = nullptr;
		}

		void attempt_save()
		{
			nlohmann::json j = *this;

			if (deep_compare(this->options, j, true))
				this->save();
		}

		bool load()
		{
			this->default_options = *this;

			std::ifstream file(settings_location);

			if (!file.is_open())
			{
				this->write_default_config();

				file.open(settings_location);
			}

			try
			{
				file >> this->options;
			}
			catch (const std::exception&)
			{
				LOG(INFO) << "Detected corrupt menu_settings, writing default config...";

				this->write_default_config();

				return this->load();
			}

			bool should_save = this->deep_compare(this->options, this->default_options);

			from_json(this->options, *this);

			if (should_save)
			{
				LOG(INFO) << "Updating menu_settings...";
				save();
			}

			return true;
		}
	private:
		std::filesystem::path settings_location;

		bool deep_compare(nlohmann::json& current_settings, const nlohmann::json& default_settings, bool compare_value = false)
		{
			bool should_save = false;

			for (auto& e : default_settings.items())
			{
				const std::string& key = e.key();

				if (current_settings.count(key) == 0 || (compare_value && current_settings[key] != e.value()))
				{
					current_settings[key] = e.value();

					should_save = true;
				}
				else if (current_settings[key].is_object() && e.value().is_object())
				{
					if (deep_compare(current_settings[key], e.value(), compare_value))
						should_save = true;
				}
				else if (!current_settings[key].is_object() && e.value().is_object())
				{
					current_settings[key] = e.value();

					should_save = true;
				}
				else if (current_settings[key].size() < e.value().size())
				{
					current_settings[key] = e.value();

					should_save = true;
				}
			}

			return should_save;
		}
		bool save()
		{
			std::ofstream file(settings_location, std::ios::out | std::ios::trunc);
			nlohmann::json j = *this;
			file << j.dump(4);
			file.close();

			return true;
		}

		bool write_default_config()
		{
			std::ofstream file(settings_location, std::ios::out);
			nlohmann::json j = *this;
			file << j.dump(4);
			file.close();

			return true;
		}
	public:
		struct player
		{
			bool invulnerable = false;
			bool inf_stamina = true;
			bool reveal_monster = true;
			bool max_sharpness = true;
			bool easy_part_breaker = false;
			bool maxed_stat_set = false;
			bool maxed_armor_effect = false;
			bool part_breaker = false;
			bool inf_item = false;
			bool inf_mount = false;
			bool mount_reset = true;
			bool inf_gathering = false;
			bool research_exp_mult = false;
			bool inf_lucky_voucher = false;
			bool cannon_always_loaded = false;
			bool inf_cannon_fire = false;
			bool unlock_all_pet = false;
			bool scoutfly_max_level = false;
			bool palico_gadget_cd = false;

			int hr_ex_mult = 1;
			int mr_ex_mult = 1;
			int palico_exp_mult = 2;

			float critical_boost = 1.25f;
			float research_exp = 2.f;

			NLOHMANN_DEFINE_TYPE_INTRUSIVE(player, 
				invulnerable, 
				inf_stamina, 
				reveal_monster, 
				inf_item, 
				hr_ex_mult, 
				critical_boost, 
				easy_part_breaker, 
				part_breaker, 
				maxed_stat_set, 
				maxed_armor_effect,
				max_sharpness,
				inf_mount,
				mount_reset,
				inf_gathering,
				research_exp_mult,
				inf_lucky_voucher,
				cannon_always_loaded,
				inf_cannon_fire,
				unlock_all_pet,
				scoutfly_max_level,
				palico_gadget_cd
			);
		} player;

		struct weapon
		{
			bool inst_chrgs_gs = true;
			bool inf_spirit_gauge = false;
			bool inf_demon_mode = false;
			bool full_charged_switch_axe = false;
			bool inf_gl_ammo = false;
			bool no_gl_cooldown = false;

			bool inf_frostcraft = false;
			bool frostcraft_recharge = false;
			bool max_slot_armor = false;
			bool inf_slinger_ammo = false;

			float frostcraft_drawn = 1.5f;
			float frostcraft_heat = 30.f;

			NLOHMANN_DEFINE_TYPE_INTRUSIVE(weapon, 
				inst_chrgs_gs, 
				inf_frostcraft, 
				frostcraft_recharge, 
				frostcraft_drawn, 
				frostcraft_heat,
				max_slot_armor,
				inf_slinger_ammo
			);
		} weapon;

		struct crafting
		{
			bool ignore_material = true;
			bool unlock_equipment = true;

			NLOHMANN_DEFINE_TYPE_INTRUSIVE(crafting, ignore_material, unlock_equipment);
		} crafting;

		struct window
		{
			ImU32 color = 3357612055;
			float gui_scale = 1.f;

			ImFont* font_title = nullptr;
			ImFont* font_sub_title = nullptr;
			ImFont* font_small = nullptr;
			ImFont* font_icon = nullptr;

			bool switched_view = true;

			NLOHMANN_DEFINE_TYPE_INTRUSIVE(window, color, gui_scale, switched_view);
		} window;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(settings, crafting, window, player, weapon);
	};
}