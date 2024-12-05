#include "hooking.hpp"
#include <script_mgr.hpp>

namespace big
{
	int __fastcall hooks::material_handler(uint32_t a1, int a2)
    {
        if (g_settings->crafting.ignore_material)
            return 99;

        return base_hook::get_original<material_handler>()(a1, a2);
    }
}