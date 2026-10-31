#include "game/features/features.hpp"
#include "game/game.hpp"

#include "core/common.hpp"
#include "core/hooking/hook_manager.hpp"
#include "core/threading/script.hpp"
#include "core/threading/script_mgr.hpp"
#include "core/ui/gui.hpp"

#include "game/events/backend_events.hpp"
#include "game/hooks/hooks.hpp"
#include "game/menu/overlay.h"
#include "game/menu/view.hpp"
#include "game/mhw/notification/notification_service.hpp"
#include "game/patches.hpp"
#include "game/pointers.hpp"

#include "core/hooking/mid_hook.hpp"

namespace game
{
	namespace
	{
		std::unique_ptr<big::pointers> g_pointer_table;
		std::unique_ptr<big::patches> g_patch_table;
	}

	void initialize()
	{
		using namespace big;

		g_pointer_table = std::make_unique<pointers>();
		LOG(INFO) << "Pointers initialized.";

		g_window = g_pointer_table->m_hwnd;

		g_patch_table = std::make_unique<patches>();
		LOG(INFO) << "Byte patches initialized.";

		// Every MHW detour and ellohim mid hook lives in game/hooks and is
		// registered through the framework API.
		install_hooks();
		LOG(INFO) << "Game hooks registered.";

		// The framework owns the presentation hooks but the game owns the
		// pointer table, so hand the addresses over here.
		if (g_hook_manager)
		{
			g_hook_manager->install_swapchain_hooks(g_pointer_table->m_swapchain_methods,
			                                        g_pointer_table->m_command_queue_execute_command_lists);
		}

		view::register_submenu();
		LOG(INFO) << "Astra submenus registered.";

		g_gui.add_tick_callback([] {
			if (features::_overlay.get_state())
				overlay::render_menu();
		});

		g_gui.set_open_callback([] {
			notification_service::success("Welcome",
			                              "Monster Hunter World Trainer Successfully Injected. Press insert to open");
		});

		g_script_mgr.add_script(std::make_unique<script>(&gui::script_func));
		g_script_mgr.add_script(std::make_unique<script>(&backend_events::script_func));
		g_script_mgr.add_script(std::make_unique<script>(&backend_events::player_event));
		LOG(INFO) << "Scripts registered.";
	}

	void enable()
	{
		big::mid_hook::enable_all();
	}

	void disable()
	{
		big::mid_hook::disable_all();
	}

	void shutdown()
	{
		big::g_script_mgr.remove_all_scripts();
		big::mid_hook::destroy_owned();

		g_patch_table.reset();
		g_pointer_table.reset();
	}
}
