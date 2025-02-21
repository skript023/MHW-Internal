#pragma once

#include <SDKDDKVer.h>
#include <Windows.h>
#include <D3D11.h>
#include <wrl/client.h>

#include <cinttypes>
#include <cstddef>
#include <cstdint>

#include <chrono>
#include <ctime>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <atomic>
#include <mutex>
#include <thread>

#include <memory>
#include <new>

#include <sstream>
#include <string>
#include <string_view>

#include <algorithm>
#include <functional>
#include <utility>

#include <stack>
#include <vector>

#include <typeinfo>
#include <type_traits>

#include <exception>
#include <stdexcept>

#include <any>
#include <optional>
#include <variant>

#include <regex>
#include <tlhelp32.h>
#include <Psapi.h>
#include <format>

#include <nlohmann/json.hpp>

#define FOLDER "Monter Hunter World Trainer"
#define LOG_FILENAME "Monster Hunter World"

#define MOD_NAME "Ellohim Private Menu"

#define PRODUCT_NAME "Ellohim"

#define DISTANCE_SPAWN 10

#define CUSTOM_HUD "Madang Hese Udud Hese Teu Boga Duit Wuhan Kontol"

#define WINDOW_CLASS "MT FRAMEWORK"
#define WINDOW_NAME "MONSTER HUNTER: WORLD(421810)"
#define TARGET_PROCESS "MonsterHunterWorld.exe"

#include "settings.hpp"
#include "logger.hpp"
#include "utility/fnv1a.hpp"

extern "C" uint64_t g_set_item;
extern "C" uint64_t g_item_box;

namespace big
{
	using namespace std::chrono_literals;
	
	template <typename T>
	using comptr = Microsoft::WRL::ComPtr<T>;

	inline HMODULE g_hmodule{};
	inline HANDLE g_main_thread{};
	inline DWORD g_main_thread_id{};
	inline std::atomic_bool g_running{ true };
}
