#include "common.hpp"
#include "fiber_pool.hpp"
#include "gui.hpp"
#include "logger.hpp"
#include "hooking.hpp"
#include "pointers.hpp"
#include "renderer.hpp"
#include "script_mgr.hpp"
#include "settings.hpp"
#include "benchmark.hpp"
#include "events/backend_events.hpp"

#include <hook_manager.hpp>
#include <gui/gui_service.hpp>
#include <meal/meal_service.hpp>
#include <world/world_service.hpp>
#include <monster/monster_service.hpp>
#include <equipment/weapon_service.hpp>
#include <equipment/armour_service.hpp>
#include <character/character_service.hpp>
#include <notification/notification_service.hpp>

DWORD APIENTRY main_thread(LPVOID)
{
	using namespace big;

	while (!FindWindow(WINDOW_CLASS, WINDOW_NAME))
		std::this_thread::sleep_for(1s);

	benchmark initialization_benchmark("Initialization");

	auto logger_instance = std::make_unique<logger>(WINDOW_NAME);
	try
	{
		LOG(RAW_GREEN_TO_CONSOLE) << R"kek(
 __  __                 _              _    _             _               __          __        _     _ 
|  \/  |               | |            | |  | |           | |           _  \ \        / /       | |   | |
| \  / | ___  _ __  ___| |_ ___ _ __  | |__| |_   _ _ __ | |_ ___ _ __(_)  \ \  /\  / /__  _ __| | __| |
| |\/| |/ _ \| '_ \/ __| __/ _ \ '__| |  __  | | | | '_ \| __/ _ \ '__|     \ \/  \/ / _ \| '__| |/ _` |
| |  | | (_) | | | \__ \ ||  __/ |    | |  | | |_| | | | | ||  __/ |   _     \  /\  / (_) | |  | | (_| |
|_|  |_|\___/|_| |_|___/\__\___|_|    |_|  |_|\__,_|_| |_|\__\___|_|  (_)     \/  \/ \___/|_|  |_|\__,_|
)kek";
		auto settings_instance = std::make_unique<settings>();
		g_settings->load();
		LOG(HACKER) << "Settings initialized.";
		
		auto pointers_instance = std::make_unique<pointers>();
		LOG(HACKER) << "Pointers initialized.";

		auto renderer_instance = std::make_unique<renderer>();
		LOG(HACKER) << "Renderer initialized.";

		auto fiber_pool_instance = std::make_unique<fiber_pool>(10);
		LOG(HACKER) << "Fiber pool initialized.";

		auto thread_pool_instance = std::make_unique<thread_pool>();
		LOG(HACKER) << "Thread Pool initialized.";

		auto hooking_instance = std::make_unique<hooking>();
		LOG(HACKER) << "Hooking initialized.";

		auto gui_service_instance = std::make_unique<gui_service>();
		auto notification_instance = std::make_unique<notification_service>();
		auto character_service_instance = std::make_unique<character_service>();
		auto armour_service_instance = std::make_unique<armour_service>();
		auto weapon_service_instance = std::make_unique<weapon_service>();
		auto meal_service_instance = std::make_unique<meal_service>();
		auto monster_service_instance = std::make_unique<monster_service>();
		auto world_service_instance = std::make_unique<world_service>();
		LOG(HACKER) << "Service registered.";

		g_script_mgr.add_script(std::make_unique<script>(&gui::script_func));
		g_script_mgr.add_script(std::make_unique<script>(&backend_events::player_event));
		g_script_mgr.add_script(std::make_unique<script>(&backend_events::weapon_event));
		g_script_mgr.add_script(std::make_unique<script>(&backend_events::script_func));
		LOG(HACKER) << "Scripts registered.";

		g_hooking->enable();
		LOG(HACKER) << "Hooking enabled.";

		auto midfunc_hook_mgr_instance = std::make_unique<hook_manager>();
		LOG(HACKER) << "Midfunction hook registered.";

		initialization_benchmark.get_runtime();
		initialization_benchmark.reset();

		while (g_running)
		{
			g_script_mgr.tick();

			std::this_thread::sleep_for(1s);
		}

		midfunc_hook_mgr_instance.reset();
		LOG(HACKER) << "Midfunction hook registered.";

		g_hooking->disable();
		LOG(HACKER) << "Hooking disabled.";

		std::this_thread::sleep_for(1000ms);

		g_script_mgr.remove_all_scripts();
		LOG(HACKER) << "Scripts unregistered.";

		notification_instance.reset();
		gui_service_instance.reset();
		character_service_instance.reset();
		armour_service_instance.reset();
		weapon_service_instance.reset();
		meal_service_instance.reset();
		monster_service_instance.reset();
		world_service_instance.reset();
		LOG(HACKER) << "Service unregistered.";

		hooking_instance.reset();
		LOG(HACKER) << "Hooking uninitialized.";

		fiber_pool_instance.reset();
		LOG(HACKER) << "Fiber pool uninitialized.";

		g_thread_pool->destroy();
		LOG(HACKER) << "Destroyed thread pool.";

		thread_pool_instance.reset();
		LOG(HACKER) << "Thread Pool uninitialized.";

		renderer_instance.reset();
		LOG(HACKER) << "Renderer uninitialized.";

		pointers_instance.reset();
		LOG(HACKER) << "Pointers uninitialized.";

		g_settings->attempt_save();
		settings_instance.reset();
		LOG(HACKER) << "Settings saved and uninitialized.";
	}
	catch (std::exception const& ex)
	{
		LOG(WARNING) << ex.what();
		MessageBoxA(nullptr, ex.what(), nullptr, MB_OK | MB_ICONEXCLAMATION);
	}

	LOG(HACKER) << "Farewell!";
	logger_instance.reset();

	CloseHandle(g_main_thread);
	FreeLibraryAndExitThread(g_hmodule, 0);

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hmod, DWORD reason, PVOID)
{
	using namespace big;

	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hmod);

		g_hmodule = hmod;
		g_main_thread = CreateThread(nullptr, 0, &main_thread, nullptr, 0, &g_main_thread_id);
		break;
	case DLL_PROCESS_DETACH:
		g_running = false;
		break;
	}

	return true;
}
