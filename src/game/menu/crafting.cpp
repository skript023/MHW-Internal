#include "game/features/ui.hpp"
#include "game/features/features.hpp"
#include "crafting.h"

#include "imgui.h"
#include "core/threading/script.hpp"

#include "game/patches.hpp"

namespace big
{
	void crafting::render_menu()
	{
		ImGui::BeginGroup();

		features::checkbox("Ignore Material", features::_ignore_material);

		features::checkbox("Unlock Equipment", features::_unlock_equipment);

		ImGui::EndGroup();
	}
}
