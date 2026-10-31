#pragma once
#include "core/common.hpp"
#include "core/utility/image_dimension.hpp"

#include <astra/host/canvas.hpp>
#include <functional>
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

		// The game layer owns its own widgets, so it registers what the shared
		// render path has to call. Keeps core free of game types.
		void add_tick_callback(std::function<void()> callback);
		void set_open_callback(std::function<void()> callback);

	public:
		bool m_opened{};
		std::vector<std::unique_ptr<banner_item>> m_banners;
		std::vector<const char*> m_banner_names;
		ImTextureID m_header{};
		ImageDimensions m_header_size = {0, 0};

	private:
		std::vector<std::function<void()>> m_tick_callbacks;
		std::function<void()> m_open_callback;
	};

	inline gui g_gui;
}
