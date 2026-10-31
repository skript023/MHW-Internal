#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "core/ui/gui.hpp"

#include "core/common.hpp"
#include "core/ui/renderer.hpp"
#include "core/threading/script.hpp"

#include <astra/host/canvas.hpp>
#include <astra/host/notification.hpp>
#include <astra/banners.hpp>
#include <imgui.h>

namespace big
{
	void gui::init()
	{
		LOG(INFO) << "Astra Submenus Registered.";
	}

	void gui::dx_init()
	{
		init();
	}

	void gui::add_tick_callback(std::function<void()> callback)
	{
		m_tick_callbacks.emplace_back(std::move(callback));
	}

	void gui::set_open_callback(std::function<void()> callback)
	{
		m_open_callback = std::move(callback);
	}

	void gui::dx_on_tick()
	{
		for (auto& callback : m_tick_callbacks)
		{
			if (callback)
				callback();
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

		int active_index = canvas::config().banner;
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
		if (m_open_callback)
			m_open_callback();
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
