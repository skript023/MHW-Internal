#include <astra/host/canvas.hpp>

namespace big
{
	bool canvas::platform_is_key_pressed(int key)
	{
		return (GetAsyncKeyState(key) & 0x8000) != 0;
	}

	bool canvas::platform_is_controller_pressed(int button)
	{
		return false;
	}

	Vector2 canvas::platform_resolution()
	{
		// The framework reads the client area of the window it hooked instead of
		// asking the game for its resolution.
		if (g_window)
		{
			if (RECT rect{}; GetClientRect(g_window, &rect) && rect.right > 0 && rect.bottom > 0)
			{
				return {static_cast<float>(rect.right), static_cast<float>(rect.bottom)};
			}
		}
		return {static_cast<float>(GetSystemMetrics(SM_CXSCREEN)), static_cast<float>(GetSystemMetrics(SM_CYSCREEN))};
	}

	void canvas::platform_prepare_render()
	{
		astra::set_srgb_output(false);
	}
}
