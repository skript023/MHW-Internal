#include "../common.hpp"
#include "range.hpp"
#include "pattern.hpp"

namespace memory
{
	range::range(handle base, std::size_t size) :
		m_base(base), m_size(size)
	{
	}

	handle range::begin()
	{
		return m_base;
	}

	handle range::end()
	{
		return m_base.add(m_size);
	}

	std::size_t range::size()
	{
		return m_size;
	}

	bool range::contains(handle h)
	{
		return h.as<std::uintptr_t>() >= begin().as<std::uintptr_t>() && h.as<std::uintptr_t>() <= end().as<std::uintptr_t>();
	}

	std::optional<handle> scan_pattern(const std::optional<uint8_t>* sig, std::size_t length, handle begin, std::size_t module_size)
	{
		std::size_t maxShift = length;
		std::size_t max_idx = length - 1;

		//Get wildcard index, and store max shiftable byte count
		std::size_t wild_card_idx{ static_cast<size_t>(-1) };
		for (int i{ static_cast<int>(max_idx - 1) }; i >= 0; --i)
		{
			if (!sig[i])
			{
				maxShift = max_idx - i;
				wild_card_idx = i;
				break;
			}
		}

		//Store max shiftable bytes for non wildcards.
		std::size_t shift_table[UINT8_MAX + 1]{};
		for (std::size_t i{}; i <= UINT8_MAX; ++i)
		{
			shift_table[i] = maxShift;
		}

		//Fill shift table with sig bytes
		for (std::size_t i{ wild_card_idx + 1 }; i != max_idx; ++i)
		{
			shift_table[*sig[i]] = max_idx - i;
		}

		//Loop data
		const auto scan_end = module_size - length;
		for (std::size_t current_idx{}; current_idx <= scan_end;)
		{
			for (std::ptrdiff_t sig_idx{ (std::ptrdiff_t)max_idx }; sig_idx >= 0; --sig_idx)
			{
				if (sig[sig_idx] && *begin.add(current_idx + sig_idx).as<uint8_t*>() != *sig[sig_idx])
				{
					current_idx += shift_table[*begin.add(current_idx + max_idx).as<uint8_t*>()];
					break;
				}
				else if (sig_idx == NULL)
				{
					return begin.add(current_idx);
				}
			}
		}
		return std::nullopt;
	}

	static bool pattern_matches(std::uint8_t* target, const std::optional<std::uint8_t>* sig, std::size_t length)
	{
		for (std::size_t i = 0; i < length; ++i)
		{
			if (sig[i] && *sig[i] != target[i])
				return false;
		}

		return true;
	};

	handle range::scan(pattern const &sig)
	{
		auto data = sig.m_bytes.data();
		auto length = sig.m_bytes.size();

		if (auto result = scan_pattern(data, length, m_base, m_size); result.has_value())
		{
			return result.value();
		}

		return nullptr;
	}

	std::vector<handle> range::scan_all(pattern const &sig)
	{
		std::vector<handle> result;

		auto data = sig.m_bytes.data();
		auto length = sig.m_bytes.size();
		for (std::uintptr_t i = 0; i < m_size - length; ++i)
		{
			if (pattern_matches(m_base.add(i).as<std::uint8_t*>(), data, length))
			{
				result.push_back(m_base.add(i));
			}
		}

		return std::move(result);
	}
}
