#pragma once
#include "file_manager.hpp"
#include "imgui.h"
#include "logger/logger.hpp"
#include "utility/utility.hpp"
#include "mhw/vector.hpp"

namespace big
{
	class menu_settings
	{
		nlohmann::json default_options;
		nlohmann::json options;

	public:
		menu_settings() = default;
		menu_settings(file setting) :
		    settings_location(setting.get_path())
		{
		}

		menu_settings* operator->() { return this; }
		const menu_settings* operator->() const { return this; }

		void set_file(file setting)
		{
			settings_location = setting.get_path();
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

			if (settings_location.empty())
				return false;

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
			if (settings_location.empty())
				return false;
			std::ofstream file(settings_location, std::ios::out | std::ios::trunc);
			nlohmann::json j = *this;
			file << j.dump(4);
			file.close();
			return true;
		}

		bool write_default_config()
		{
			if (settings_location.empty())
				return false;
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

			NLOHMANN_DEFINE_TYPE_INTRUSIVE(player, invulnerable, inf_stamina, reveal_monster, inf_item, hr_ex_mult, critical_boost, easy_part_breaker, part_breaker, maxed_stat_set, maxed_armor_effect, max_sharpness, inf_mount, mount_reset, inf_gathering, research_exp_mult, inf_lucky_voucher, cannon_always_loaded, inf_cannon_fire, unlock_all_pet, scoutfly_max_level, palico_gadget_cd);
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

			bool cb_phials = false;
			bool cb_sword_charge = false;
			bool cb_shield_charge = false;
			bool ig_extract = false;
			bool ig_stamina = false;
			bool ig_jump = false;
			bool bow_coatings = false;
			bool bow_charge = false;
			bool bowgun_no_reload = false;
			bool inf_wyvern_blast = false;
			bool inf_wyvernheart = false;
			bool wyvernheart_rapidfire = false;
			bool bowgun_no_recoil = false;

			NLOHMANN_DEFINE_TYPE_INTRUSIVE(weapon, inst_chrgs_gs, inf_spirit_gauge, inf_demon_mode, full_charged_switch_axe, inf_gl_ammo, no_gl_cooldown, inf_frostcraft, frostcraft_recharge, frostcraft_drawn, frostcraft_heat, max_slot_armor, inf_slinger_ammo, cb_phials, cb_sword_charge, cb_shield_charge, ig_extract, ig_stamina, ig_jump, bow_coatings, bow_charge, bowgun_no_reload, inf_wyvern_blast, inf_wyvernheart, wyvernheart_rapidfire, bowgun_no_recoil);
		} weapon;

		struct crafting
		{
			bool ignore_material = true;
			bool unlock_equipment = true;

			NLOHMANN_DEFINE_TYPE_INTRUSIVE(crafting, ignore_material, unlock_equipment);
		} crafting;

		struct window
		{
			ImU32 color        = 3357612055;
			float gui_scale    = 1.f;
			float transparency = 1.f;
			Color custom_background{8, 12, 20, 255};
			Color custom_panel{15, 24, 38, 255};
			Color custom_text{225, 235, 250, 255};

			ImFont* font_title     = nullptr;
			ImFont* font_sub_title = nullptr;
			ImFont* font_small     = nullptr;
			ImFont* font_icon      = nullptr;

			int layout         = 0; // 0: List, 1: Window
			int theme          = 4; // Emerald, Violet, Ocean
			int banner         = 0; // Index into astra::banners().
			bool switched_view = true;
			bool mouse_active  = false;
			bool input         = false;
			bool censor        = true;
			bool overlay       = false;
			bool overlay_monster = true;
			bool overlay_crown   = true;
			bool overlay_enrage_bar = true;
			bool overlay_hunter  = true;
			bool js_eval       = false;

			uint32_t open_key{VK_INSERT};
			uint32_t back_key{VK_NUMPAD0};
			uint32_t enter_key{VK_NUMPAD5};
			uint32_t up_key{VK_NUMPAD8};
			uint32_t down_key{VK_NUMPAD2};
			uint32_t left_key{VK_NUMPAD4};
			uint32_t right_key{VK_NUMPAD6};
			uint32_t left_tab_key{VK_NUMPAD7};
			uint32_t right_tab_key{VK_NUMPAD9};

			Vector2 m_pos{100.f, 100.f};
			float m_width = 450.f;
			std::size_t m_option_per_page = 11;

			Color m_submenu_bar_background_color{20, 20, 20, 255};
			Color m_submenu_bar_text_color{255, 255, 255, 255};

			Color m_toggle_on_color{0, 255, 0, 255};
			Color m_toggle_off_color{255, 0, 0, 255};

			Color m_submenu_rect_color{255, 255, 255, 255};

			Color m_option_selected_background_color{255, 255, 255, 255};
			Color m_option_unselected_background_color{0, 0, 0, 160};

			Color m_option_selected_text_color{0, 0, 0, 255};
			Color m_option_unselected_text_color{255, 255, 255, 255};

			Color m_footer_background_color{20, 20, 20, 255};

			Color m_tab_selected_color{80, 140, 240, 255};
			Color m_tab_unselected_color{0, 0, 0, 160};
			Color m_tab_selected_text_color{255, 255, 255, 255};
			Color m_tab_unselected_text_color{255, 255, 255, 255};

			Color description_background_color{0, 0, 0, 160};
			Color description_text_color{255, 255, 255, 255};

			int m_open_delay = 200;
			int m_back_delay = 200;
			int m_enter_delay = 200;
			int m_vectical_delay = 120;
			int m_horizontal_delay = 120;
			int m_tabbar_switch = 150;
			bool m_sounds = false;

			NLOHMANN_DEFINE_TYPE_INTRUSIVE(window, layout, theme, banner, color, gui_scale, transparency, custom_background, custom_panel, custom_text, mouse_active, censor, overlay, overlay_monster, overlay_crown, overlay_enrage_bar, overlay_hunter, open_key, back_key, enter_key, up_key, down_key, left_key, right_key, left_tab_key, right_tab_key, m_pos, m_width, m_option_per_page, m_sounds, m_open_delay, m_enter_delay, m_back_delay, m_horizontal_delay, m_vectical_delay, m_tabbar_switch, m_submenu_bar_background_color, m_submenu_bar_text_color, m_toggle_on_color, m_toggle_off_color, m_submenu_rect_color, m_option_selected_background_color, m_option_unselected_background_color, m_option_selected_text_color, m_option_unselected_text_color, m_footer_background_color, m_tab_selected_color, m_tab_unselected_color, m_tab_selected_text_color, m_tab_unselected_text_color, description_background_color, description_text_color)
		} window;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(menu_settings, crafting, window, player, weapon);
	};

	inline menu_settings g_settings{};
}
