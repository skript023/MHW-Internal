#include "core/common.hpp"
#include "core/hooking/hook_manager.hpp"
#include "core/memory/module.hpp"
#include "core/ui/renderer.hpp"

#include <astra/host/canvas.hpp>
#include <ellohim/logger.hpp>

namespace big
{
	hook_manager::hook_manager()
	{
		ellohim::logger::set_callback([](ellohim::log_level level, std::string_view message) {
			if (level == ellohim::log_level::info)
			{
				LOG(INFO) << "[Ellohim] " << message;
			}
			else
			{
				LOG(WARNING) << "[Ellohim] " << message;
			}
		});

		detour_hook::add<hooks::set_cursor_pos>("SetCursorPos", memory::module("user32.dll").get_export("SetCursorPos").as<void*>());
		detour_hook::add<hooks::convert_thread_to_fiber>(
		    "ConvertThreadToFiber", memory::module("kernel32.dll").get_export("ConvertThreadToFiber").as<void*>());

		g_hook_manager = this;
	}

	hook_manager::~hook_manager()
	{
		if (m_enabled)
			disable();

		delete_disabled_hooks();

		g_hook_manager = nullptr;
	}

	void hook_manager::install_swapchain_hooks(void** swapchain_methods, void* execute_command_lists_target)
	{
		if (!swapchain_methods)
		{
			LOG(WARNING) << "No swapchain method table; presentation hooks are skipped.";
			return;
		}

		detour_hook::add<hooks::swapchain_present>("SwapChainPresent", swapchain_methods[hooks::swapchain_present_index]);
		detour_hook::add<hooks::swapchain_resizebuffers>("SwapChainResizeBuffers", swapchain_methods[hooks::swapchain_resizebuffers_index]);

		if (execute_command_lists_target)
		{
			detour_hook::add<hooks::execute_command_lists>("ExecuteCommandLists", execute_command_lists_target);
		}
	}

	void hook_manager::delete_disabled_hooks()
	{
		std::vector<detour_base*> disabled;
		for (auto* hook : detour_base::hooks())
		{
			if (!hook->is_enabled())
			{
				disabled.push_back(hook);
			}
		}
		for (auto* hook : disabled)
		{
			delete hook;
		}
	}

	void hook_manager::enable()
	{
		if (!g_window)
			throw std::runtime_error("Cannot install the window hook: no target window.");

		m_og_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(g_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&hooks::wndproc)));

		try
		{
			if (!detour_base::enable_all())
			{
				throw std::runtime_error("Could not enable all hooks");
			}
			m_enabled = true;
		}
		catch (...)
		{
			SetWindowLongPtrW(g_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_og_wndproc));
			detour_base::disable_all();
			throw;
		}
	}

	void hook_manager::disable()
	{
		m_enabled = false;
		SetWindowLongPtrW(g_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_og_wndproc));
		detour_base::disable_all();
		for (auto* hook : detour_base::hooks())
		{
			if (hook->is_enabled())
			{
				LOG(WARNING) << "Hook '" << hook->name() << "' is covered by another program's hook; it stays in place.";
			}
		}
		delete_disabled_hooks();
	}

	void* hooks::convert_thread_to_fiber(void* param)
	{
		if (IsThreadAFiber())
		{
			return GetCurrentFiber();
		}

		return base_hook::get_original<hooks::convert_thread_to_fiber>()(param);
	}

	LRESULT hooks::wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (g_running)
		{
			if (g_renderer)
				g_renderer->wndproc(hwnd, msg, wparam, lparam);
			if (auto result = canvas::block_game_message(hwnd, msg, wparam, lparam))
			{
				return *result;
			}
		}

		return CallWindowProcW(g_hook_manager->m_og_wndproc, hwnd, msg, wparam, lparam);
	}

	BOOL hooks::set_cursor_pos(int x, int y)
	{
		if (canvas::uses_mouse())
			return true;

		return base_hook::get_original<hooks::set_cursor_pos>()(x, y);
	}
}
