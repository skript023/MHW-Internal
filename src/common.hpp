#pragma once

#include <D3D11.h>
#include <Psapi.h>
#include <SDKDDKVer.h>
#include <Windows.h>
#include <algorithm>
#include <any>
#include <atomic>
#include <chrono>
#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <exception>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <new>
#include <nlohmann/json.hpp>
#include <optional>
#include <regex>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <tlhelp32.h>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <variant>
#include <vector>
#include <wrl/client.h>

#define FOLDER "Monter Hunter World Trainer"
#define LOG_FILENAME "Monster Hunter World"

#define MOD_NAME "Ellohim Private Menu"

#define PRODUCT_NAME "Ellohim"

#define DISTANCE_SPAWN 10

#define CUSTOM_HUD "Madang Hese Udud Hese Teu Boga Duit Wuhan Kontol"

#define WINDOW_CLASS "MT FRAMEWORK"
#define WINDOW_NAME "MONSTER HUNTER: WORLD(421810)"
#define TARGET_PROCESS "MonsterHunterWorld.exe"

#include "logger/logger.hpp"
#include "settings.hpp"
#include "utility/fnv1a.hpp"

extern "C" uint64_t g_set_item;
extern "C" uint64_t g_item_box;

namespace big
{
	using namespace std::chrono_literals;

	template<typename T>
	using comptr = Microsoft::WRL::ComPtr<T>;

	inline HMODULE g_hmodule{};
	inline HANDLE g_main_thread{};
	inline DWORD g_main_thread_id{};
	inline std::atomic_bool g_running{true};
}
