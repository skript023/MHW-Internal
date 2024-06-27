#include "imgui.h"
#include "script.hpp"
#include "player_menu.h"
#include "settings.hpp"
#include "utility/mhw.hpp"
#include <midfunc_hook_manager.hpp>

namespace big
{
	void player_menu::render_menu()
	{
        ImGui::BeginGroup();

        if (ImGui::Checkbox("Invulnerable", &g_settings->player.invulnerable))
			player_menu::invulnerable();

        if (ImGui::Checkbox("Inf Stamina", &g_settings->player.inf_stamina))
			player_menu::infinite_stamina();

        ImGui::EndGroup();
	}
    void player_menu::invulnerable()
	{
		if (g_settings->player.invulnerable)
		{
			g_hook_manager->m_player_anim.apply();
			g_hook_manager->m_player_state.apply();
			g_hook_manager->m_player_health.apply();
		}
		else
		{
			g_hook_manager->m_player_anim.restore();
			g_hook_manager->m_player_state.restore();
			g_hook_manager->m_player_health.restore();
		}
	}
	void player_menu::infinite_stamina()
	{
		if (g_settings->player.inf_stamina)
		{
			g_hook_manager->m_player_stamina.apply();
		}
		else
		{
			g_hook_manager->m_player_stamina.restore();
		}
	}
}