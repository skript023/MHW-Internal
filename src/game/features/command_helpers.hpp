#pragma once
#include "core/commands/bool_command.hpp"

#include <functional>
#include <string>

namespace big::features
{
	// bool_command whose enable/disable callbacks do the real work (byte patch
	// apply/restore). Features only need to pass the two lambdas.
	class patch_toggle : public bool_command
	{
		std::function<void()> m_enable;
		std::function<void()> m_disable;

	public:
		patch_toggle(std::string name, std::string label, std::string description, bool def_value,
		             std::function<void()> on_enable, std::function<void()> on_disable) :
		    bool_command(std::move(name), std::move(label), std::move(description), def_value),
		    m_enable(std::move(on_enable)),
		    m_disable(std::move(on_disable))
		{
		}

	protected:
		void on_enable() override
		{
			if (m_enable)
				m_enable();
		}
		void on_disable() override
		{
			if (m_disable)
				m_disable();
		}
	};
}
