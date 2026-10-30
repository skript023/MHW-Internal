#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "gui.hpp"

#include "common.hpp"
#include "menu/view.hpp"
#include "pointers.hpp"
#include "renderer.hpp"
#include "menu_settings.hpp"
#include "script.hpp"
#include "services/notification/notification_service.hpp"
#include "menu/overlay.h"

#include <astra/host/canvas.hpp>
#include <astra/host/notification.hpp>
#include <astra/banners.hpp>
#include <imgui.h>

namespace big
{
	void gui::init()
	{
		view::register_submenu();
		LOG(INFO) << "Astra Submenus Registered.";
	}

	void gui::dx_init()
	{
		init();
	}

	void gui::dx_on_tick()
	{
		if (g_settings.window.overlay)
		{
			overlay::render_menu();
		}
	}

	void gui::dx_on_opened()
	{
		update_banner();
		canvas::tick();
		g_gui.m_opened = canvas::is_opened();
	}

	bool gui::load_banner(const std::string& name, const unsigned char* bytes, int size)
	{
		if (!g_renderer)
			return false;

		int width = 0, height = 0;
		auto pixels = stbi_load_from_memory(bytes, size, &width, &height, nullptr, 4);
		if (!pixels)
		{
			LOG(WARNING) << "Cannot decode menu texture for banner: " << name;
			return false;
		}

		auto id = g_renderer->upload_rgba(pixels, width, height);
		stbi_image_free(pixels);

		if (!id)
		{
			LOG(WARNING) << "Cannot upload menu texture for banner: " << name;
			return false;
		}

		auto banner = std::make_unique<banner_item>();
		banner->name = name;
		banner->id = id;
		banner->dimensions = {width, height};

		m_banner_names.push_back(banner->name.c_str());
		m_banners.push_back(std::move(banner));

		update_banner();
		return true;
	}

	void gui::load_textures()
	{
		m_banners.clear();
		m_banner_names.clear();

		for (const auto& banner : astra::banners())
		{
			load_banner(std::string(banner.name), banner.image.data(), static_cast<int>(banner.image.size()));
		}

		update_banner();
		LOG(INFO) << "Loaded " << m_banners.size() << " Astra banners.";
	}

	void gui::update_banner()
	{
		if (m_banners.empty())
		{
			m_header      = 0;
			m_header_size = {0, 0};
			return;
		}

		int active_index = g_settings.window.banner;
		if (active_index < 0 || static_cast<std::size_t>(active_index) >= m_banners.size())
		{
			active_index = 0;
		}

		m_header      = m_banners[active_index]->id;
		m_header_size = m_banners[active_index]->dimensions;
	}

	void gui::script_init()
	{
		g_gui.m_opened = true;
		notification_service::success("Welcome", "Monster Hunter World Trainer Successfully Injected. Press insert to open");
	}

	void gui::script_on_tick()
	{
	}

	void gui::script_func()
	{
		while (true)
		{
			g_gui.script_on_tick();
			script::get_current()->yield();
		}
	}
}
