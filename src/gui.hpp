#pragma once
#include "common.hpp"

#include <astra/canvas.hpp>

namespace big
{
	class gui
	{
	public:
		void dx_init();
		void dx_on_tick();
		void dx_on_opened();

		void script_init();
		void script_on_tick();
		static void script_func();

	public:
		bool m_opened{};
		bool m_astra_opened{true};
		astra::canvas m_astra_canvas;
	};

	inline gui g_gui;
} // namespace big
