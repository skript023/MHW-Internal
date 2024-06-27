#include "common.hpp"
#include "function_types.hpp"
#include "logger.hpp"
#include "gui.hpp"
#include "hooking.hpp"
#include "memory/module.hpp"
#include "pointers.hpp"
#include "renderer.hpp"

#include <MinHook.h>

namespace big
{
	hooking::hooking() :
		m_swapchain_present_hook("SwapChainPresent", g_pointers->m_swapchain_methods[hooks::swapchain_present_index], &hooks::swapchain_present),
		m_swapchain_resizebuffers_hook("SwapChainResizeBuffers", g_pointers->m_swapchain_methods[hooks::swapchain_resizebuffers_index], &hooks::swapchain_resizebuffers),
		m_set_cursor_pos_hook("SetCursorPos", memory::module("user32.dll").get_export("SetCursorPos").as<void*>(), &hooks::set_cursor_pos),
		m_convert_thread_to_fiber_hook("ConvertThreadToFiber", memory::module("kernel32.dll").get_export("ConvertThreadToFiber").as<void*>(), &hooks::convert_thread_to_fiber),
		m_ignore_material_hook("Ignore Material", g_pointers->m_ignore_material, &hooks::material_handler),
		m_unlock_equipment_hook("Unlock Equipment", g_pointers->m_equipment_unlock, &hooks::equipment_crafting)
	{
		g_hooking = this;
	}

	hooking::~hooking()
	{
		if (m_enabled)
			disable();

		g_hooking = nullptr;
	}

	void hooking::enable()
	{
		m_swapchain_present_hook.enable();
		m_swapchain_resizebuffers_hook.enable();
		m_og_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(g_pointers->m_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&hooks::wndproc)));
		m_set_cursor_pos_hook.enable();
		m_convert_thread_to_fiber_hook.enable();

		m_ignore_material_hook.enable();
		m_unlock_equipment_hook.enable();

		m_enabled = true;
	}

	void hooking::disable()
	{
		m_enabled = false;

		m_ignore_material_hook.disable();
		m_unlock_equipment_hook.disable();

		m_convert_thread_to_fiber_hook.disable();
		m_set_cursor_pos_hook.disable();
		SetWindowLongPtrW(g_pointers->m_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_og_wndproc));
		m_swapchain_resizebuffers_hook.disable();
		m_swapchain_present_hook.disable();
	}

	minhook_keepalive::minhook_keepalive()
	{
		MH_Initialize();
	}

	minhook_keepalive::~minhook_keepalive()
	{
		MH_Uninitialize();
	}

	void* hooks::convert_thread_to_fiber(void* param)
	{
		if (IsThreadAFiber())
		{
			return GetCurrentFiber();
		}

		return g_hooking->m_convert_thread_to_fiber_hook.get_original<decltype(&convert_thread_to_fiber)>()(param);
	}

	LRESULT hooks::wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (g_running)
		{
			g_renderer->wndproc(hwnd, msg, wparam, lparam);
		}

		return CallWindowProcW(g_hooking->m_og_wndproc, hwnd, msg, wparam, lparam);
	}

	BOOL hooks::set_cursor_pos(int x, int y)
	{
		if (g_gui.m_opened)
			return true;

		return g_hooking->m_set_cursor_pos_hook.get_original<decltype(&set_cursor_pos)>()(x, y);
	}
}
