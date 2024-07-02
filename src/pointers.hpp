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
		HWND m_hwnd{};
		void* m_swapchain_methods[19];
	public:
		iVector2* m_resolution{};
		uint64_t m_base_address{};
		void* m_ignore_material{};
		void* m_equipment_unlock{};
	public:
		void* m_part_breaker{};
		void* m_easy_breaker{};
		void* m_reveal_monster{};
		uintptr_t** m_player_stat{};
		void* m_player_state{};
		void* m_player_anim{};
		void* m_player_health{};
		void* m_player_stamina{};
		void* m_consumable{};
		void* m_use_item{};
		void* m_master_rank_exp{};
		void* m_highrank_exp{};
		void* m_maxed_effect_set{};
		void* m_maxed_attribute{};
		void* m_max_sharpness{};
		void* m_gs_inst_chrgs{};
		void* m_inf_frostcraft{};
		void* m_frostcraft_drawn{};
		void* m_frostcraft_heat{};
		void* m_inf_gathering{};
		void* m_gathering_resource{};
		void* m_mount_reset{};
		void* m_inf_mount{};
		void* m_research_exp{};
		void* m_lucky_voucher{};
	private:
		functions::create_d3d11_device_and_swapchain_t create_device_and_swapchain{};
		bool get_swapchain();
		void window_focus();
		bool swapchain_found = false;
		HWND m_window;
		IDXGISwapChain* m_swapchain = nullptr;
		ID3D11Device* m_d3d_device = nullptr;
		ID3D11DeviceContext* m_d3d_context = nullptr;
		DWORD m_process_id;
		HANDLE m_handle;
		LPCSTR m_title;
		LPCSTR m_classname;
		LPCSTR m_path;
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
