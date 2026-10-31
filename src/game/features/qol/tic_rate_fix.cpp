#include "game/features/features.hpp"

namespace big::features
{
	bool_command _tic_rate_fix{"tic_rate_fix", "Tic Rate Fix (High-FPS)", "Fix multi-hit tick damage on >60FPS/144Hz/240Hz monitors.", true};
}
