#include "crafting.h"

#include "imgui.h"
#include "core/threading/script.hpp"
#include "game/settings.hpp"

#include "game/patches.hpp"

namespace big
{
	void crafting::render_menu()
	{
		ImGui::BeginGroup();

		ImGui::Checkbox("Ignore Material", &g_settings->crafting.ignore_material);

		ImGui::Checkbox("Unlock Equipment", &g_settings->crafting.unlock_equipment);

		ImGui::EndGroup();
	}
}
