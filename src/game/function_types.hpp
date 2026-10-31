#pragma once
#include "core/common.hpp"

#include "game/classes/fwddec.hpp"

namespace big::functions
{
	using use_item_t = void(__fastcall*)(Consumeable* rax, int a2, int8_t a3);
	using select_weapon_t = __int64(__fastcall*)(uint32_t a1, uint32_t a2);
	using select_armour_t = Equipment*(__fastcall*)(uint32_t a1, uint32_t a2);
	using bow_gun_effect_apply_t = int(__fastcall*)(__int64 a1);
}
