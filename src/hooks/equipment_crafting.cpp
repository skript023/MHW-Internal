#include "hooking.hpp"

namespace big
{
    bool __fastcall hooks::equipment_crafting(__int64 a1)
    {
        if (g_settings->crafting.unlock_equipment)
            return true;

        return base_hook::get_original<equipment_crafting>()(a1);
    }
}