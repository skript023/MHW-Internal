#pragma once
#include "common.hpp"
#include "function_types.hpp"
#include <class/vector.hpp>

namespace big
{
	class pointers
	{
	public:
		explicit pointers();
		~pointers();
	public:
		iVector2* m_resolution{};
		uint64_t m_base_address{};
		void* m_ignore_material{};
		void* m_equipment_unlock{};
	public:
		HWND m_hwnd{};
		void* m_swapchain_methods[19];
		void* m_part_breaker{};
		void* m_easy_breaker{};
		void* m_reveal_monster{};
		uintptr_t** m_critical_boost{};
		void* m_player_state{};
		void* m_player_anim{};
		void* m_player_health{};
		void* m_player_stamina{};
		void* m_consumable{};
		void* m_use_item{};
		void* m_highrank_exp{};
	private:
		functions::create_d3d11_device_and_swapchain_t create_device_and_swapchain{};
		bool get_swapchain();
		bool swapchain_found = false;
		HWND m_window;
		IDXGISwapChain* m_swapchain = nullptr;
		ID3D11Device* m_d3d_device = nullptr;
		ID3D11DeviceContext* m_d3d_context = nullptr;
	public:
		inline DWORD get_process_id(LPCSTR ProcessName)
		{
			PROCESSENTRY32 pt;
			HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			pt.dwSize = sizeof(PROCESSENTRY32);
			if (Process32First(hsnap, &pt))
			{
				do
				{
					if (!lstrcmpi(pt.szExeFile, ProcessName))
					{
						CloseHandle(hsnap);
						return pt.th32ProcessID;
					}
				} while (Process32Next(hsnap, &pt));
			}
			CloseHandle(hsnap);
			return 0;
		}
	};

	inline pointers *g_pointers{};
}
