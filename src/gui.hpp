#pragma once
#include "common.hpp"
#include "class/image_dimension.hpp"

#include <astra/host/canvas.hpp>
#include <memory>
#include <string>
#include <vector>

namespace big
{
	struct banner_item
	{
		std::string name;
		ImTextureID id{};
		ImageDimensions dimensions{};
	};

	class gui
	{
	public:
		void init();
		void dx_init();
		void dx_on_tick();
		void dx_on_opened();

		void script_init();
		void script_on_tick();
		static void script_func();

		void load_textures();
		bool load_banner(const std::string& name, const unsigned char* bytes, int size);
		void update_banner();

	public:
		bool m_opened{};
		std::vector<std::unique_ptr<banner_item>> m_banners;
		std::vector<const char*> m_banner_names;
		ImTextureID m_header{};
		ImageDimensions m_header_size = {0, 0};
	};

	inline gui g_gui;
}
