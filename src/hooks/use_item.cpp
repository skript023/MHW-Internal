#include "hooking.hpp"

namespace big
{
	void __fastcall hooks::use_item(__int64 rcx, int a2, int8_t a3)
	{
		if (g_settings->player.inf_item)
		{
            LOG(INFO) << "Value of A1 is " << std::hex << rcx;
            LOG(INFO) << "Value of A2 is " << a2;
            LOG(INFO) << "Value of A3 is " << (uint32_t)a3;

            if (*(DWORD*)(rcx + 0x8) > 0xCC)
            {
                LOG(INFO) << "rcx + 8 = 0xCC returned to original value is : " << *(DWORD*)(rcx + 0x8);
                if (a3 != 0x01)
                {
                    return;
                }
            }
            else if (a3 != 0x00) 
            {
                return;
            }
		}

        return g_hooking->m_use_item_hook.get_original<decltype(&hooks::use_item)>()(rcx, a2, a3);
	}
}