#include "core/commands/commands.hpp"
#include "core/common.hpp"
#include "core/config/file_manager.hpp"
#include "core/hooking/hook_manager.hpp"
#include "core/logging/logger.hpp"
#include "core/net/server_module.hpp"
#include "core/threading/fiber_pool.hpp"
#include "core/threading/script_mgr.hpp"
#include "core/threading/task_mgr.hpp"
#include "core/threading/thread_pool.hpp"
#include "core/ui/renderer.hpp"
#include "core/utility/benchmark.hpp"

#include <astra/host/settings.hpp>

#include "game/game.hpp"

#include <openssl/crypto.h>

namespace
{
	HWND wait_for_target_window()
	{
		using namespace big;

		do
		{
			if (auto window = FindWindowA(config::target_window_class, config::target_window_name))
			{
				return window;
			}
			std::this_thread::sleep_for(100ms);
		} while (g_running);

		return nullptr;
	}
}

DWORD APIENTRY main_thread(LPVOID)
{
	using namespace big;

	g_window = wait_for_target_window();
	if (!g_window)
	{
		LOG(WARNING) << "Injection cancelled before the target window appeared.";
		return 0;
	}

	benchmark initialization_benchmark("Initialization");

	std::filesystem::path base_dir = std::getenv("appdata");
	base_dir /= config::data_folder;

	file_manager::init(base_dir);

	logger::initialize(config::log_file, file_manager::get_project_file(std::format("./{}.log", config::log_file)), true);
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
		settings::initialize(file_manager::get_project_file("./settings.json"));
		LOG(INFO) << "Settings initialized.";

		auto renderer_instance = std::make_unique<renderer>();
		LOG(INFO) << "Renderer initialized.";

		auto fiber_pool_instance = std::make_unique<fiber_pool>(10);
		LOG(INFO) << "Fiber pool initialized.";

		auto thread_pool_instance = std::make_unique<thread_pool>();
		LOG(INFO) << "Thread pool initialized.";

		// Framework-owned hooks. The game layer hands over the presentation
		// addresses while it initializes.
		auto hooking_instance = std::make_unique<hook_manager>();
		LOG(INFO) << "Hooking initialized.";

		game::initialize();

		g_hook_manager->enable();
		game::enable();
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
			task_mgr::tick();
			g_script_mgr.tick();
			settings::apply_external();
			settings::tick();

			std::this_thread::sleep_for(10ms);
		}

		commands::shutdown();
		LOG(INFO) << "Commands shutdown.";

		game::disable();
		LOG(INFO) << "Game hooks disabled.";

		g_hook_manager->disable();
		LOG(INFO) << "Hooking disabled.";

		std::this_thread::sleep_for(500ms);

		game::shutdown();
		LOG(INFO) << "Game layer uninitialized.";

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

		settings::tick();
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
