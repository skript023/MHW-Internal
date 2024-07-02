#include "hooking.hpp"

namespace big
{
	void __fastcall hooks::use_item(__int64* rcx, unsigned char a2, int8_t a3)
	{
		if (g_settings->player.inf_item)
		{
        //    // Custom hook logic based on original assembly code
        //    LOG(INFO) << "RCX + 0x8 : " << *(int*)(rcx + 0x08);
        //    LOG(INFO) << "RCX + 0xC : " << *(int*)(rcx + 0x0C);

        //    if (*(rcx + 0x08) > 0xCC)
        //        goto code2;
        //    if (r8l != 0x00)
        //        goto end;

        //    goto return2;

        //code2:
        //    if (r8l != 0x01)
        //        goto end;

        //return2:
        //    return g_hooking->m_use_item_hook.get_original<decltype(&hooks::use_item)>()(rcx, r8l);

        //end:
        //    return g_hooking->m_use_item_hook.get_original<decltype(&hooks::use_item)>()(rcx, r8l);

            if (*(DWORD*)(rcx + 0x8) > 0xCC)
            {
                goto label;
            }

            // Check condition: cmp r8l,00
            if (a3 != 0x00) 
            {
                return g_hooking->m_use_item_hook.get_original<decltype(&hooks::use_item)>()(rcx, a2, a3);
            }

        label:
            if (a3 != 0x01)
            {
                return g_hooking->m_use_item_hook.get_original<decltype(&hooks::use_item)>()(rcx, a2, a3);
            }
		}

        return g_hooking->m_use_item_hook.get_original<decltype(&hooks::use_item)>()(rcx, a2, a3);
	}
}