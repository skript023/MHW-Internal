#include <astra/host/canvas.hpp>
#include "pointers.hpp"

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
		if (g_pointers && g_pointers->m_resolution)
			return {static_cast<float>(g_pointers->m_resolution->x), static_cast<float>(g_pointers->m_resolution->y)};
		return {1920.f, 1080.f};
	}

	void canvas::platform_prepare_render()
	{
		astra::set_srgb_output(false);
	}
}
