#pragma once
#include "core/utility/utility.hpp"
#include "core/utility/vector.hpp"

#include <astra/host/state_serializer.hpp>
#include <imgui.h>

namespace big
{
	// Menu cosmetics and key bindings only. Feature state lives in the command
	// registry (game/features) and is persisted by the "commands" component.
	class menu_settings : public state_serializer
	{
	public:
		struct window_settings
		{
			ImU32 color        = 3357612055;
			float gui_scale    = 1.f;
			float transparency = 1.f;
			Color custom_background{13, 15, 19, 245};
			Color custom_panel{20, 22, 27, 245};
			Color custom_text{242, 234, 222, 255};

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

			Color m_submenu_rect_color{185, 148, 88, 255};

			Color m_option_selected_background_color{255, 255, 255, 255};
			Color m_option_unselected_background_color{0, 0, 0, 160};

			Color m_option_selected_text_color{255, 215, 95, 255};
			Color m_option_unselected_text_color{255, 255, 255, 255};

			Color m_footer_background_color{20, 20, 20, 255};

			Color m_tab_selected_color{205, 165, 95, 255};
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

			NLOHMANN_DEFINE_TYPE_INTRUSIVE(window_settings, layout, theme, banner, color, gui_scale, transparency, custom_background, custom_panel, custom_text, mouse_active, censor, open_key, back_key, enter_key, up_key, down_key, left_key, right_key, left_tab_key, right_tab_key, m_pos, m_width, m_option_per_page, m_sounds, m_open_delay, m_enter_delay, m_back_delay, m_horizontal_delay, m_vectical_delay, m_tabbar_switch, m_submenu_bar_background_color, m_submenu_bar_text_color, m_toggle_on_color, m_toggle_off_color, m_submenu_rect_color, m_option_selected_background_color, m_option_unselected_background_color, m_option_selected_text_color, m_option_unselected_text_color, m_footer_background_color, m_tab_selected_color, m_tab_unselected_color, m_tab_selected_text_color, m_tab_unselected_text_color, description_background_color, description_text_color)
		
		};

		window_settings window;

		menu_settings() :
		    state_serializer("menu")
		{
		}

		void save_state_impl(nlohmann::json& state) override
		{
			state = window;
		}

		void load_state_impl(nlohmann::json& state) override
		{
			if (state.is_object())
				from_json(state, window);
		}
	};

	inline menu_settings g_settings{};
}
