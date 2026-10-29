#include "benchmark.hpp"
#include "common.hpp"
#include "events/backend_events.hpp"
#include "fiber_pool.hpp"
#include "thread_pool.hpp"
#include "file_manager.hpp"
#include "gui.hpp"
#include "hooking.hpp"
#include "pointers.hpp"
#include "renderer.hpp"
#include "script_mgr.hpp"
#include "menu_settings.hpp"
#include "commands/commands.hpp"
#include "features/features.hpp"

#include <character/character_service.hpp>
#include <equipment/armour_service.hpp>
#include <equipment/weapon_service.hpp>
#include <hook_manager.hpp>
#include <meal/meal_service.hpp>
#include <monster/monster_service.hpp>
#include <notification/notification_service.hpp>
#include <openssl/crypto.h>
#include <server/server_module.hpp>
#include <world/world_service.hpp>

DWORD APIENTRY main_thread(LPVOID)
{
	using namespace big;

	while (!FindWindow(WINDOW_CLASS, WINDOW_NAME))
		std::this_thread::sleep_for(1s);

	benchmark initialization_benchmark("Initialization");

	std::filesystem::path base_dir = std::getenv("appdata");
	base_dir /= FOLDER;

	g_file_manager.init(base_dir);

	logger::initialize(LOG_FILENAME, g_file_manager.get_project_file(std::format("./{}.log", FOLDER)), true);
	try
	{
		LOG(INFO) << R"kek(
 __  __                 _              _    _             _               __          __        _     _ 
|  \/  |               | |            | |  | |           | |           _  \ \        / /       | |   | |
| \  / | ___  _ __  ___| |_ ___ _ __  | |__| |_   _ _ __ | |_ ___ _ __(_)  \ \  /\  / /__  _ __| | __| |
| |\/| |/ _ \| '_ \/ __| __/ _ \ '__| |  __  | | | | '_ \| __/ _ \ '__|     \ \/  \/ / _ \| '__| |/ _` |
| |  | | (_) | | | \__ \ ||  __/ |    | |  | | |_| | | | | ||  __/ |   _     \  /\  / (_) | |  | | (_| |
|_|  |_|\___/|_| |_|___/\__\___|_|    |_|  |_|\__,_|_| |_|\__\___|_|  (_)     \/  \/ \___/|_|  |_|\__,_|
)kek";
		g_settings.set_file(g_file_manager.get_project_file("./settings.json"));
		g_settings.load();
		LOG(INFO) << "Settings initialized.";

		auto pointers_instance = std::make_unique<pointers>();
		LOG(INFO) << "Pointers initialized.";

		auto renderer_instance = std::make_unique<renderer>();
		LOG(INFO) << "Renderer initialized.";

		auto fiber_pool_instance = std::make_unique<fiber_pool>(10);
		LOG(INFO) << "Fiber pool initialized.";

		auto thread_pool_instance = std::make_unique<thread_pool>();
		LOG(INFO) << "Thread pool initialized.";

		auto hooking_instance = std::make_unique<hooking>();
		LOG(INFO) << "Hooking initialized.";

		auto character_service_instance = std::make_unique<character_service>();
		auto armour_service_instance    = std::make_unique<armour_service>();
		auto weapon_service_instance    = std::make_unique<weapon_service>();
		auto monster_service_instance   = std::make_unique<monster_service>();
		auto world_service_instance     = std::make_unique<world_service>();
		auto notification_instance      = std::make_unique<notification_service>();
		LOG(INFO) << "Services initialized.";

		auto midfunc_hook_mgr_instance = std::make_unique<hook_manager>();
		LOG(INFO) << "Byte patch manager initialized.";

		g_script_mgr.add_script(std::make_unique<script>(&gui::script_func));
		g_script_mgr.add_script(std::make_unique<script>(&backend_events::script_func));
		g_script_mgr.add_script(std::make_unique<script>(&backend_events::player_event));
		g_script_mgr.add_script(std::make_unique<script>(&backend_events::weapon_event));
		LOG(INFO) << "Scripts registered.";

		g_hooking->enable();
		LOG(INFO) << "Hooking enabled.";

#ifdef PRODUCTION
		auto server_instance = std::make_unique<server_module>();
		LOG(INFO) << "Server module initialized.";
#endif

		initialization_benchmark.get_runtime();

		commands::enable_bool_commands();
		LOG(INFO) << "Commands enabled.";

		while (g_running)
		{
#ifdef PRODUCTION
			if (g_server_module && !g_server_module->authorized())
			{
				LOG(WARNING) << "[License] DLL session revoked or expired; shutting down safely.";
				g_running = false;
				break;
			}
#endif
			commands::run_looped_command();
			g_script_mgr.tick();
			g_settings->attempt_save();

			std::this_thread::sleep_for(10ms);
		}

		commands::shutdown();
		LOG(INFO) << "Commands shutdown.";

		midfunc_hook_mgr_instance.reset();
		LOG(INFO) << "Byte patches restored.";

		g_hooking->disable();
		LOG(INFO) << "Hooking disabled.";

		std::this_thread::sleep_for(500ms);

		g_script_mgr.remove_all_scripts();
		LOG(INFO) << "Scripts unregistered.";

		notification_instance.reset();
		character_service_instance.reset();
		armour_service_instance.reset();
		weapon_service_instance.reset();
		monster_service_instance.reset();
		world_service_instance.reset();
		LOG(INFO) << "Service unregistered.";

#ifdef PRODUCTION
		server_instance.reset();
		LOG(INFO) << "Disconnected from server.";
#endif

		hooking_instance.reset();
		LOG(INFO) << "Hooking uninitialized.";

		fiber_pool_instance.reset();
		LOG(INFO) << "Fiber pool uninitialized.";

		g_thread_pool->destroy();
		LOG(INFO) << "Destroyed thread pool.";

		thread_pool_instance.reset();
		LOG(INFO) << "Thread pool uninitialized.";

		renderer_instance.reset();
		LOG(INFO) << "Renderer uninitialized.";

		pointers_instance.reset();
		LOG(INFO) << "Pointers uninitialized.";

		g_settings.attempt_save();
		LOG(INFO) << "Settings saved and uninitialized.";
	}
	catch (std::exception const& ex)
	{
		LOG(WARNING) << ex.what();
		MessageBoxA(nullptr, ex.what(), nullptr, MB_OK | MB_ICONEXCLAMATION);
	}

	if (!detour_base::hooks().empty())
	{
		LOG(WARNING) << "DLL retained because another program still calls into its hooks. Restart the game before injecting again.";
		return 0;
	}

	OPENSSL_cleanup();
	LOG(INFO) << "Farewell!";
	logger::destroy();

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

		g_hmodule     = hmod;
		g_main_thread = CreateThread(nullptr, 0, &main_thread, nullptr, 0, &g_main_thread_id);
		break;
	case DLL_PROCESS_DETACH:
		g_running = false;
		break;
	}

	return true;
}
