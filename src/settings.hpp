#pragma once
#include "logger.hpp"
#include "imgui.h"

namespace big
{
	class settings;
	inline settings* g_settings{};

	class settings 
	{
		nlohmann::json default_options;
		nlohmann::json options;
	public:
		struct player
		{
			bool invulnerable = true;
			bool inf_stamina = true;
			bool reveal_monster = false;
			bool max_sharpness = false;
			bool easy_part_breaker = false;
			bool maxed_stat_set = false;
			bool maxed_armor_effect = false;
			bool part_breaker = false;
			bool inf_item = false;
			int hr_ex_mult = 1;
			int mr_ex_mult = 1;
			float critical_boost = 1.25f;

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
				max_sharpness
			);
		} player;

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
	public:
		settings()
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

			std::string settings_file = std::getenv("appdata");
			settings_file += this->settings_location;

			std::ifstream file(settings_file);

			if (!file.is_open())
			{
				this->write_default_config();

				file.open(settings_file);
			}

			try
			{
				file >> this->options;
			}
			catch (const std::exception&)
			{
				LOG(HACKER) << "Detected corrupt settings, writing default config...";

				this->write_default_config();

				return this->load();
			}

			bool should_save = this->deep_compare(this->options, this->default_options);

			from_json(this->options, *this);

			if (should_save)
			{
				LOG(HACKER) << "Updating settings...";
				save();
			}

			return true;
		}

	private:
		const char* settings_location = "\\" FOLDER "\\settings.json";

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
				else if (current_settings[key].is_structured() && e.value().is_structured())
				{
					if (deep_compare(current_settings[key], e.value(), compare_value))
						should_save = true;
				}
				else if (!current_settings[key].is_structured() && e.value().is_structured()) {
					current_settings[key] = e.value();

					should_save = true;
				}
			}

			return should_save;
		}

		bool save()
		{
			std::string settings_file = std::getenv("appdata");
			settings_file += this->settings_location;

			std::ofstream file(settings_file, std::ios::out | std::ios::trunc);
			file << default_options.dump(4);
			file.close();

			return true;
		}

		bool write_default_config()
		{
			std::string settings_file = std::getenv("appdata");
			settings_file += this->settings_location;

			std::ofstream file(settings_file, std::ios::out);
			file << default_options.dump(4);
			file.close();

			return true;
		}

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(settings, crafting, window, player);
	};
}