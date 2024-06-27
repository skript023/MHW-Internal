#include "crafting.h"
#include "imgui.h"
#include "script.hpp"
#include "settings.hpp"
#include <hook_manager.hpp>

namespace big
{
	void crafting::render_menu()
	{
		ImGui::BeginGroup();

		if (ImGui::Checkbox("Reveal Monster", &g_settings->player.reveal_monster))
			reveal_monster();

		ImGui::EndGroup();
	}
	void crafting::reveal_monster()
	{
		if (g_settings->player.reveal_monster)
			g_hook_manager->m_reveal_monster.apply();
		else
			g_hook_manager->m_reveal_monster.restore();
	}
}