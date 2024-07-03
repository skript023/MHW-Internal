#include "common.hpp"
#include "logger.hpp"
#include "pointers.hpp"
#include "memory/all.hpp"

namespace big
{
	pointers::pointers(): m_resolution(new iVector2(1920, 1080)), m_base_address(memory::module(nullptr).begin().as<uint64_t>())
	{
		memory::pattern_batch main_batch;

		if (!this->get_swapchain())
			LOG(WARNING) << "Failed get swapchain";

		main_batch.add("Ignore Material", "40 57 48 83 EC 20 8B F9 83 FA 02", [this](memory::handle ptr)
		{
			m_ignore_material = ptr.as<void*>();
		});
		
		main_batch.add("Unlock Equipment", "48 89 5C 24 20 55 56 41 54 41 55 41 56 48 83 EC 20 48 8B E9 48 85 C9", [this](memory::handle ptr)
		{
			m_equipment_unlock = ptr.as<void*>();
		});
		
		main_batch.add("Part Breaker", "F3 41 0F 11 84 24 A8 01 00 00", [this](memory::handle ptr)
		{
			m_part_breaker = ptr.as<void*>();
		});
		
		main_batch.add("Easy Part Breaker", "F3 0F 10 41 10 C3", [this](memory::handle ptr)
		{
			m_easy_breaker = ptr.as<void*>();
		});
		
		main_batch.add("Show All Monster", "F3 0F 10 77 28 48", [this](memory::handle ptr)
		{
			m_reveal_monster = ptr.as<void*>();
		});
		
		main_batch.add("Player Stat", "48 8B 05 ? ? ? ? F3 0F 10 88 ? ? ? ? 0F 2F C1 77 03 0F 28 C1", [this](memory::handle ptr)
		{
			m_player_stat = ptr.add(3).rip().as<uintptr_t**>();
		});
		
		main_batch.add("Player State", "E8 EB 6C 2C FF", [this](memory::handle ptr)
		{
			m_player_state = ptr.as<void*>();
		});
		
		main_batch.add("Player Anim", "E8 2E 08 00 00 44", [this](memory::handle ptr)
		{
			m_player_anim = ptr.as<void*>();
		});
		
		main_batch.add("Player Health", "E8 6C 26 D3 00", [this](memory::handle ptr)
		{
			m_player_health = ptr.as<void*>();
		});
		
		main_batch.add("Player Health", "F3 0F 5D C2 F3 0F 11 41 6C", [this](memory::handle ptr)
		{
			m_player_stamina = ptr.as<void*>();
		});
		
		main_batch.add("Consumable", "48 89 5C 24 ? 89 4C 24 08 55 56 57 41 56 41 57 48 8B EC 48 83 EC 30", [this](memory::handle ptr)
		{
			m_consumable = ptr.as<void*>();
		});
		
		main_batch.add("Consumable Use", "89 51 0C 85 D2 75 09 41 80 F8 01 75 03 89 51 08", [this](memory::handle ptr)
		{
			m_use_item = ptr.as<decltype(m_use_item)>();
		});
		
		main_batch.add("Master Rank EXP Mult", "41 57 48 83 EC 20 48 8B F1 41 0F B6 F8 48 83 C1 08", [this](memory::handle ptr)
		{
			m_master_rank_exp = ptr.sub(24).as<void*>();
		});
		
		main_batch.add("High Rank EXP Mult", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 54 41 56 41 57 48 83 EC 20 48 8B F9 8B F2 48 83 C1 08", [this](memory::handle ptr)
		{
			m_highrank_exp = ptr.as<void*>();
		});
		
		main_batch.add("Maxed Set Effect", "0F B6 48 31 00 8C D3 80 00 00 00", [this](memory::handle ptr)
		{
			m_maxed_effect_set = ptr.as<void*>();
		});
		
		main_batch.add("Maxed Armor Attribute", "43 0F B6 44 34 31", [this](memory::handle ptr)
		{
			m_maxed_attribute = ptr.as<void*>();
		});
		
		main_batch.add("Max Sharpness", "8B B9 F8 20 00 00", [this](memory::handle ptr)
		{
			m_max_sharpness = ptr.as<void*>();
		});
		
		main_batch.add("Great Sword Instant Charges", "89 B3 58 23 00 00 C6", [this](memory::handle ptr)
		{
			m_gs_inst_chrgs = ptr.as<void*>();
		});
		
		main_batch.add("Inf Frostcraft", "F3 0F 5C C1 0F 2F C2 72", [this](memory::handle ptr)
		{
			m_inf_frostcraft = ptr.as<void*>();
		});
		
		main_batch.add("Frostcraft Recharge Drawn", "F3 0F 59 04 C8 F3 0F 58", [this](memory::handle ptr)
		{
			m_frostcraft_drawn = ptr.as<void*>();
		});
		
		main_batch.add("Frostcraft Recharge Heat", "F3 0F 59 84 C8 34 07 00 00", [this](memory::handle ptr)
		{
			m_frostcraft_heat = ptr.as<void*>();
		});
		
		main_batch.add("Inf Gathering", "40 53 48 83 EC 30 80 B9 ? ? ? ? ? 48 8B DA F2 0F 10 05", [this](memory::handle ptr)
		{
			m_inf_gathering = ptr.as<void*>();
		});
		
		main_batch.add("Gathering Resource", "FF C8 41 89 80 DC 00 00 00", [this](memory::handle ptr)
		{
			m_gathering_resource = ptr.as<void*>();
		});
		
		main_batch.add("Mount Reset", "F3 0F 10 81 70 C1 01 00 C3", [this](memory::handle ptr)
		{
			m_mount_reset = ptr.as<void*>();
		});
		
		main_batch.add("Inf Reset", "73 06 F3 0F 11 51 38", [this](memory::handle ptr)
		{
			m_inf_mount = ptr.as<void*>();
		});
		
		main_batch.add("Reserch EXP Multiplier", "48 89 5C 24 ? 55 56 57 41 55 41 57 48 83 EC 40 0F 29 74 24", [this](memory::handle ptr)
		{
			m_research_exp = ptr.as<void*>();
		});
		
		main_batch.add("Lucky Voucher", "3B B0 54 91 26 00 73 23", [this](memory::handle ptr)
		{
			m_lucky_voucher = ptr.as<void*>();
		});
		
		main_batch.run(memory::module(nullptr));

		this->m_hwnd = FindWindow(WINDOW_CLASS, WINDOW_NAME);
		if (!this->m_hwnd)
			throw std::runtime_error("Failed to find the game's window.");

		g_pointers = this;
	}

	pointers::~pointers()
	{
		g_pointers = nullptr;
	}

	bool pointers::get_swapchain()
	{
		WNDCLASSEX windowClass;
		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = DefWindowProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = GetModuleHandle(NULL);
		windowClass.hIcon = NULL;
		windowClass.hCursor = NULL;
		windowClass.hbrBackground = NULL;
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = "UnrealWindow";
		windowClass.hIconSm = NULL;

		::RegisterClassEx(&windowClass);

		this->m_window = ::CreateWindow(windowClass.lpszClassName, "UnrealWindow", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, windowClass.hInstance, NULL);

		if (this->m_window == NULL)
		{
			return false;
		}

		HMODULE d3d11 = ::GetModuleHandle("d3d11.dll");
		if (d3d11 == NULL)
		{
			::DestroyWindow(this->m_window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
			return false;
		}

		void* create_device_and_swapchain = ::GetProcAddress(d3d11, "D3D11CreateDeviceAndSwapChain");
		if (create_device_and_swapchain == NULL)
		{
			::DestroyWindow(this->m_window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
			return false;
		}

		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2, D3D_FEATURE_LEVEL_9_1, };

		DXGI_RATIONAL refreshRate;
		refreshRate.Numerator = 60;
		refreshRate.Denominator = 1;

		DXGI_MODE_DESC bufferDesc;
		bufferDesc.Width = 100;
		bufferDesc.Height = 100;
		bufferDesc.RefreshRate = refreshRate;
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SAMPLE_DESC sampleDesc;
		sampleDesc.Count = 1;
		sampleDesc.Quality = 0;

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		swapChainDesc.BufferDesc = bufferDesc;
		swapChainDesc.SampleDesc = sampleDesc;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = this->m_window;
		swapChainDesc.Windowed = 1;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT hr = ((functions::create_d3d11_device_and_swapchain_t)(create_device_and_swapchain))(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 2, D3D11_SDK_VERSION, &swapChainDesc, &this->m_swapchain, &this->m_d3d_device, &featureLevel, &this->m_d3d_context);
		
		if (FAILED(hr))
		{
			::DestroyWindow(this->m_window);
			::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
			return false;
		}

		::memcpy(this->m_swapchain_methods, *(void***)this->m_swapchain, sizeof(m_swapchain_methods));

		this->m_swapchain->Release();
		this->m_swapchain = nullptr;

		this->m_d3d_device->Release();
		this->m_d3d_device = nullptr;

		this->m_d3d_context->Release();
		this->m_d3d_context = nullptr;

		::DestroyWindow(this->m_window);
		::UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

		return true;
	}
	void pointers::window_focus()
	{
		bool window_focus = false;

		while (window_focus == false)
		{
			DWORD forground_process_id;
			GetWindowThreadProcessId(GetForegroundWindow(), &forground_process_id);
			if (GetCurrentProcessId() == forground_process_id) 
			{

				m_process_id = GetCurrentProcessId();
				m_handle = GetCurrentProcess();
				m_hwnd = GetForegroundWindow();

				RECT TempRect;
				GetWindowRect(m_hwnd, &TempRect);
				m_resolution->x = TempRect.right - TempRect.left;
				m_resolution->y = TempRect.bottom - TempRect.top;

				char TempTitle[MAX_PATH];
				GetWindowText(m_hwnd, TempTitle, sizeof(TempTitle));
				m_title = TempTitle;

				char TempClassName[MAX_PATH];
				GetClassName(m_hwnd, TempClassName, sizeof(TempClassName));
				m_classname = TempClassName;

				char TempPath[MAX_PATH];
				GetModuleFileNameExA(m_handle, NULL, TempPath, sizeof(TempPath));
				m_path = TempPath;

				window_focus = true;
			}
		}
	}
}