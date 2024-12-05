#include "base.hpp"

namespace big
{
	base_hook::base_hook(const std::string_view name) :
		m_name(name),
		m_enabled(false)
	{
		m_base_hooks.emplace_back(this);
	}

	std::vector<base_hook*>& base_hook::hooks()
	{
		return m_base_hooks;
	}

	bool base_hook::enable_all()
	{
		bool status = true;
		for (auto base_hook : m_base_hooks)
		{
			status = base_hook->enable() && status;
		}
		return status;
	}

	bool base_hook::disable_all()
	{
		bool status = true;
		for (auto base_hook : m_base_hooks)
		{
			status = base_hook->disable() && status;
		}
		return status;
	}
}