#include "common.hpp"
#include "logger.hpp"
#include "pointers.hpp"
#include "memory/all.hpp"

uint64_t g_equipment_ret_addr;
uint64_t g_gs_charge_ret_addr;
uint64_t g_atk_ret_addr;
uint64_t g_def_ret_addr;
uint64_t g_affinity_ret_addr;
uint64_t g_affinity_call_addr;
uint64_t g_selected_item_ret_addr;

namespace big
{
	pointers::pointers(): m_resolution(new iVector2(1920, 1080)), m_base_address(memory::module(nullptr).begin().as<uint64_t>())
	{
		memory::pattern_batch main_batch;

		if (!this->get_swapchain())
			LOG(WARNING) << "Failed get swapchain";

		/*main_batch.add("Screen Resolution", "48 8D 05 ? ? ? ? 4C 89 74 24 ? 48 89 44 24 ? 0F 57 D2 48 8D 05", [this](memory::handle ptr)
		{
			m_resolution = ptr.as<iVector2*>();
		});
		*/
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
		
		main_batch.add("Load Cannon", "83 BF B8 0F 00 00 00", [this](memory::handle ptr)
		{
			m_loaded_cannon = ptr.as<void*>();
		});
		
		main_batch.add("Cannon Fire", "7F 0A C7 83 D0 0E 00 00 07 00 00 00", [this](memory::handle ptr)
		{
			m_cannon_fire = ptr.as<void*>();
		});
		
		main_batch.add("Barrel Placement Max", "7F 0A C7 83 D0 0E 00 00 07 00 00 00", [this](memory::handle ptr)
		{
			m_barrel_placement_max = ptr.as<void*>();
		});
		
		main_batch.add("Barrel Placement Count", "7F 0A C7 83 D0 0E 00 00 07 00 00 00", [this](memory::handle ptr)
		{
			m_barrel_placement_count = ptr.as<void*>();
		});
		
		main_batch.add("Pet Manager", "41 8B 84 97 28 6F 0F 00", [this](memory::handle ptr)
		{
			m_pet_mgr = ptr.as<void*>();
		});
		
		main_batch.add("Pet Unlocker", "8B 84 96 28 6F 0F 00 48", [this](memory::handle ptr)
		{
			m_pet_unlocker = ptr.as<void*>();
		});
		
		main_batch.add("Max Slot Armor", "41 0F B7 48 1E F3", [this](memory::handle ptr)
		{
			m_max_slot_armor = ptr.as<void*>();
		});
		
		main_batch.add("Max Slot Armor Ranged", "0F B6 46 33 89 83 44 1D 00 00", [this](memory::handle ptr)
		{
			m_max_slot_armor_ranged = ptr.as<void*>();
		});
		
		main_batch.add("Max Slot Armor Melee", "0F B6 46 2D 89 83 44 1D 00 00", [this](memory::handle ptr)
		{
			m_max_slot_armor_melee = ptr.as<void*>();
		});
		
		main_batch.add("Scoutfly Max", "0F B6 46 2D 89 83 44 1D 00 00", [this](memory::handle ptr)
		{
			m_scoutfly_max = ptr.as<void*>();
		});
		
		main_batch.add("Scoutfly Level", "0F B6 46 2D 89 83 44 1D 00 00", [this](memory::handle ptr)
		{
			m_scoutfly_level = ptr.as<void*>();
		});
		
		main_batch.add("Palico Gadget Cooldown", "F3 0F 10 84 81 40 A3 00 00 F3", [this](memory::handle ptr)
		{
			m_palico_gadget_cd = ptr.as<void*>();
		});
		
		main_batch.add("Slinger Ammo", "41 83 BC C9 20 7E 00 00 01", [this](memory::handle ptr)
		{
			m_slinger_ammo = ptr.as<void*>();
		});
		
		main_batch.add("Slinger Max Ammo", "C3 C7 41 08 00 00 00 00 41", [this](memory::handle ptr)
		{
			m_slinger_max_ammo = ptr.as<void*>();
		});
		
		main_batch.add("Highlighted Armour", "45 0F BE 47 2C 48 8D 15 ? ? ? ? 48 8D 4D 87", [this](memory::handle ptr)
		{
			m_armor_ptr = ptr.sub(175).as<void*>();
		});
		
		main_batch.add("Highlight Armour", "8B C1 83 FA FF 74 20 3B 15", [this](memory::handle ptr)
		{
			m_select_armour = ptr.as<decltype(m_select_armour)>();
		});
		
		main_batch.add("Highlighted Weapon", "0F B6 40 20 48 83 C4 20 5B C3 8B", [this](memory::handle ptr)
		{
			m_weapon_ptr = ptr.sub(61).as<void*>();
		});
		
		main_batch.add("Highlight Weapon", "E8 ? ? ? ? EB 05 E8 ? ? ? ? 48 85 C0 74 28 0F", [this](memory::handle ptr)
		{
			m_select_waapon = ptr.add(1).rip().as<decltype(m_select_waapon)>();
		});
		
		main_batch.add("Atk Ptr", "48 8B 7B 08 45 0F 57 C9", [this](memory::handle ptr)
		{
			m_atk_ptr = ptr.as<void*>();
			g_atk_ret_addr = ptr.add(5).as<uint64_t>();
		});
		
		main_batch.add("Def Ptr", "44 2B C1 44 3B C7 73 05", [this](memory::handle ptr)
		{
			m_def_ptr = ptr.as<void*>();
			g_def_ret_addr = ptr.add(5).as<uint64_t>();
		});
		
		main_batch.add("Affinity Ptr", "49 8B 4E 08 48 8B 89 20 7D 00 00 E8", [this](memory::handle ptr)
		{
			m_affinity_ptr = ptr.add(11).as<void*>();
			g_affinity_ret_addr = ptr.add(11 + 5).as<uint64_t>();
		});
		
		main_batch.add("Affinity Ptr", "40 ? 48 83 EC ? 48 8B ? ? 48 8B ? 4C 8B ? ? ? ? ? 4D 85 ? 0F 84 ? ? ? ? 41 F6 40 0C ? 0F 84 ? ? ? ? 49 63 ? ? ? ? ? 0F 29 ? ? ? 66 41 ? ? ? ? ? ? ? 0F 5B ? 85 D2 7E ? 49 8B ? ? ? ? ? 32 C0 48 85 ? 74 ? 80 79 1C ? 0F 93 ? EB ? 49 8B ? ? ? ? ? 48 85 ? 74 ? 80 79 20 ? 41 B9 ? ? ? ? 0F B6 ? 41 0F ? ? 84 C0 48 8B ? ? ? ? ? 74 ? 0F B6 ? ? ? ? ? ? 66 0F ? ? EB ? 0F B6 ? ? ? ? ? ? 66 0F ? ? 0F 5B ? F3 0F ? ? 49 8B ? E8 ? ? ? ? 8B C0 0F 57 ? F3 48 ? ? ? 48 8B ? ? 48 8B ? ? ? ? ? F3 0F ? ? 48 85 ? 74 ? F6 42 0C ? 75 ? 33 D2 B9 ? ? ? ? E8 ? ? ? ? 0F 57 ? 8B C0 F3 48 ? ? ? F3 0F ? ? 0F 28 ? ? ? 48 83 C4", [this](memory::handle ptr)
		{
			g_affinity_call_addr = ptr.as<uint64_t>();
		});
		
		main_batch.add("Character Ptr", "8B 8F A0 00 00 00 8B C1 F7", [this](memory::handle ptr)
		{
			m_character_ptr = ptr.sub(30).as<void*>();
		});
		
		main_batch.add("Player Params", "48 8B 05 ? ? ? ? F3 0F 10 80 34 01 00 00 C3", [this](memory::handle ptr)
		{
			m_player_params = ptr.add(3).rip().as<decltype(m_player_params)>();
		});
		
		main_batch.add("Item Params", "48 8B 05 ? ? ? ? 44 8B F6 F3 0F 10 3D", [this](memory::handle ptr)
		{
			m_item_params = ptr.add(3).rip().as<void*>();
		});
		
		main_batch.add("Skill Params", "48 03 0D ? ? ? ? 48 8D 14 9B 0F B6 84 0A", [this](memory::handle ptr)
		{
			m_skill_params = ptr.add(3).rip().as<void*>();
		});
		
		main_batch.add("Mantle Params", "48 8B 05 ? ? ? ? F3 0F 10 80 EC 00 00 00 0F 2F 87", [this](memory::handle ptr)
		{
			m_mantle_params = ptr.add(3).rip().as<void*>();
		});
		
		main_batch.add("Player Data", "48 8B 0D ? ? ? ? 4C 8D 43 50 48 8B D3", [this](memory::handle ptr)
		{
			m_player_data = ptr.add(3).rip().as<void*>();
		});
		
		main_batch.add("World Data", "48 8B 05 ? ? ? ? F3 0F 10 40 38 0F 2F 40 50 72", [this](memory::handle ptr)
		{
			m_world_data = ptr.add(3).rip().as<void*>();
		});
		
		main_batch.add("Session Data", "48 8B 3D ? ? ? ? BB FF FF FF FF 41 8B 87 0C 29 00 00", [this](memory::handle ptr)
		{
			m_session_data = ptr.add(3).rip().as<void*>();
		});
		
		main_batch.add("Gathering Data", "48 8B 0D ? ? ? ? E8 ? ? ? ? 48 85 C0 75 0E 48 8B 0D ? ? ? ? 33 D2", [this](memory::handle ptr)
		{
			m_gathering_data = ptr.add(3).rip().as<void*>();
		});
		
		main_batch.add("Monster Data", "48 8B 05 ? ? ? ? 48 8B 97 B0 0D 00 00", [this](memory::handle ptr)
		{
			m_monster_data = ptr.add(3).rip().as<decltype(m_monster_data)>();
		});
		
		main_batch.add("Item Caps", "48 8B 0D ? ? ? ? 48 85 C9 74 ? E8 ? ? ? ? 48 85 C0 74 ? 0F BE 40 12", [this](memory::handle ptr)
		{
			//m_character_ptr = ptr.add(3).rip().as<void*>();
		});
		
		main_batch.add("Save Data", "48 8B 05 ? ? ? ? 48 8D 4C 24 30 48 8B 90 A8 00 00 00 46 8B 84 22 94 00 00 00 48 8D 15", [this](memory::handle ptr)
		{
			//m_character_ptr = ptr.add(3).rip().as<void*>();
		});
		
		main_batch.add("Steamworks", "48 8B 05 ? ? ? ? 83 B8 40 03 00 00 00 75 ? 80 B8", [this](memory::handle ptr)
		{
			//m_character_ptr = ptr.add(3).rip().as<void*>();
		});
		
		main_batch.add("Coating Data", "48 8B 0D ? ? ? ? E8 ? ? ? ? 48 85 C0 74 ? 0F B6 48 08", [this](memory::handle ptr)
		{
			//m_character_ptr = ptr.add(3).rip().as<void*>();
		});
		
		main_batch.add("World Session", "48 8B 05 ? ? ? ? 83 B8 D4 01 00 00 03 7D ? F3", [this](memory::handle ptr)
		{
			//m_character_ptr = ptr.add(3).rip().as<void*>();
		});
		
		main_batch.add("Session Quest", "48 8B 0D ? ? ? ? 44 8B F0 8B 99 40 A3 02 00 E8 ? ? ? ? 84 C0", [this](memory::handle ptr)
		{
			//m_character_ptr = ptr.add(3).rip().as<void*>();
		});
		
		main_batch.add("Harvest Box Data", "48 8B 05 ? ? ? ? 48 8B 7C 24 40 48 8B B4 24 88 00 00 00", [this](memory::handle ptr)
		{
			//m_character_ptr = ptr.add(3).rip().as<void*>();
		});
		
		main_batch.add("Armor Skill", "48 8B 0D ? ? ? ? 8B D0 E8 ? ? ? ? 0F BF 44 78 24 48 8B 5C 24 30", [this](memory::handle ptr)
		{
			//m_character_ptr = ptr.add(3).rip().as<void*>();
		});
		
		main_batch.add("Current Player Name", "48 8B 0D ? ? ? ? 48 8D 55 97 41 B1 01 45 33 C0 E8", [this](memory::handle ptr)
		{
			m_current_player_name = ptr.as<void*>();
		});
		
		main_batch.add("Current Weapon", "48 8B 0D ? ? ? ? 4C 8D 45 ? 48 8D 97 ? ? ? ? E8 ? ? ? ? 48 8B D8 48 85 C0", [this](memory::handle ptr)
		{
			m_current_player_name = ptr.as<void*>();
		});
		
		main_batch.add("Player Damage", "48 8B 0D ? ? ? ? E8 ? ? ? ? 48 8B D8 48 85 C0 75 04 33 C9", [this](memory::handle ptr)
		{
			m_current_player_name = ptr.as<void*>();
		});
		
		main_batch.add("Player Name", "48 8B 0D ? ? ? ? 48 8D 54 24 38 C6 44 24 20 00 E8 ? ? ? ? 48 8B 5C 24 70 48 8B 7C 24 60 48 83 C4 68 C3", [this](memory::handle ptr)
		{
			m_current_player_name = ptr.as<void*>();
		});
		
		main_batch.add("Monster", "48 8B 0D ? ? ? ? B2 01 E8 ? ? ? ? C6 83 ? ? ? ? ? 48 8B 0D", [this](memory::handle ptr)
		{
			m_monster = ptr.as<void*>();
		});
		
		main_batch.add("Player Buff", "48 8B 05 ? ? ? ? 41 8B 94 00 ? ? ? ? 89 57 ?", [this](memory::handle ptr)
		{
			m_player_buff = ptr.as<void*>();
		});
		
		main_batch.add("Selected Monster", "48 8B 0D ? ? ? ? E8 ? ? ? ? 48 8B 0D ? ? ? ? E8 ? ? ? ? 48 8B 05 ? ? ? ? 83 A0 ? ? ? ? ? C6 43 ? ?", [this](memory::handle ptr)
		{
			m_selected_monster = ptr.as<void*>();
		});
		
		main_batch.add("Lobby Status", "48 8B 0D ? ? ? ? E8 ? ? ? ? 48 8B 4E ? F3 0F 10 86 ? ? ? ? F3 0F 58 86 ? ? ? ? F3 0F 11 86 ? ? ? ? E8 ? ? ? ? 48 8B 4E ?", [this](memory::handle ptr)
		{
			m_lobby_status = ptr.as<void*>();
		});
		
		main_batch.add("Damage on Screen", "48 8B 0D ? ? ? ? 45 33 C0 48 8B D0 E8 ? ? ? ? 48 8B F0 48 39 83 ? ? ? ?", [this](memory::handle ptr)
		{
			m_dmg_on_screen = ptr.as<void*>();
		});
		
		main_batch.add("Meal Handler", "49 89 6B 08 48 8D 42", [this](memory::handle ptr)
		{
			m_meal_handler = ptr.sub(15).as<void*>();
		});
		
		main_batch.add("Palico Experience Handler", "41 03 C6 3B C5 0F 47 C5 89 81 60", [this](memory::handle ptr)
		{
			m_palico_experiece_handler = ptr.sub(61).as<void*>();
		});
		
		main_batch.add("Player Position", "48 89 5C 24 ? 55 56 41 57 48 8D AC 24", [this](memory::handle ptr)
		{
			m_player_position = ptr.as<void*>();
		});
		
		main_batch.add("Player Coordinates", "48 8B C4 55 53 57 41 55 41 56 48 8D 68 A1 48 81 EC", [this](memory::handle ptr)
		{
			m_player_forward = ptr.as<void*>();
		});
		
		main_batch.add("Waypoint", "40 55 48 83 EC 50 48 89 5C 24 ? 48 8B E9 48", [this](memory::handle ptr)
		{
			m_waypoint = ptr.as<void*>();
		});
		
		main_batch.add("Spirit Gauge", "F3 0F 10 87 68 23 00 00 0F", [this](memory::handle ptr)
		{
			m_spirit_gauge = ptr.as<void*>();
		});
		
		main_batch.add("Demon Mode", "F3 0F 10 87 6C 23 00 00 F3 0F 5E", [this](memory::handle ptr)
		{
			m_demon_mode = ptr.as<void*>();
		});
		
		main_batch.add("Switch Axe", "F3 0F 10 B1 50 23 00 00", [this](memory::handle ptr)
		{
			m_fully_charged_switch_axe = ptr.as<void*>();
		});
		
		main_batch.add("GL Ammo", "8B 82 6C 23 00 00 89 81", [this](memory::handle ptr)
		{
			m_gl_ammo = ptr.as<void*>();
		});
		
		main_batch.add("GL Cooldown", "F3 0F 11 4B 10 77", [this](memory::handle ptr)
		{
			m_gl_cooldown = ptr.as<void*>();
		});
		
		main_batch.run(memory::module(nullptr));

		this->m_hwnd = this->window_focus();
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
	HWND pointers::window_focus()
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

				RECT rect;
				GetWindowRect(m_hwnd, &rect);
				m_resolution->x = rect.right - rect.left;
				m_resolution->y = rect.bottom - rect.top;

				char title[MAX_PATH];
				GetWindowText(m_hwnd, title, sizeof(title));
				m_title = title;

				char className[MAX_PATH];
				GetClassName(m_hwnd, className, sizeof(className));
				m_classname = className;

				char path[MAX_PATH];
				GetModuleFileNameExA(m_handle, NULL, path, sizeof(path));
				m_path = path;

				window_focus = true;

				return m_hwnd;
			}
		}

		return nullptr;
	}
}