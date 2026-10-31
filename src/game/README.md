# `game`

Everything Monster Hunter: World specific. This is the only layer allowed to
know about the game's pointer table, offsets, features and menus.

- `game.cpp` — `game::initialize()` / `game::enable()` / `game::disable()` /
  `game::shutdown()`, the single entry point the app calls
- `pointers.{hpp,cpp}` — pattern-scanned pointer table (no swapchain: the
  framework's `core/ui/graphic/graphic_manager` owns the dummy device and the
  IDXGISwapChain / ID3D12CommandQueue method tables)
- `patches.{hpp,cpp}` — game byte patches (`g_patches`)
- `settings.hpp` — menu cosmetics and key bindings only (`g_settings`), an Astra
  `state_serializer` component named `menu`
- `classes/` — in-memory game classes (`fwddec.hpp`) and math types shared with the game ABI
- `hooks/` — game detours, ellohim mid hooks, shellcode stubs and the registration list
  (`install.cpp`, `hooks.hpp`)
- `features/` — one command per feature (see below)
- `menu/` — Astra submenus, overlay and notification widgets
- `mhw/` — game services (camera, character, damage, drop, equipment, meal,
  monster, notification, party, qol, world)
- `events/` — tick scripts bound to features and services

## Features are commands

Feature state lives in the command registry, not in a settings object. Every
feature is one translation unit under `game/features/<group>/` that defines a
single command and registers itself in its constructor:

```cpp
// game/features/player/inf_stamina.cpp
#include "game/features/command_helpers.hpp"
#include "game/features/features.hpp"
#include "game/patches.hpp"

namespace big::features
{
	namespace
	{
		void enable()  { if (g_patches) g_patches->m_player_stamina.apply(); }
		void disable() { if (g_patches) g_patches->m_player_stamina.restore(); }
	}

	patch_toggle _inf_stamina{"inf_stamina", "Infinite Stamina", "Stamina never depletes.", true, enable, disable};
}
```

Command kinds:

| kind | type | use |
|---|---|---|
| `patch_toggle` | `bool_command` + two callbacks | applies/restores byte patches on enable/disable |
| `bool_command` | plain toggle | a flag that hooks or other features read with `get_state()` |
| `number_command<int>` | number with min/max | EXP multipliers and other integers |
| `float_command` | float with min/max | multipliers (frostcraft, carve speed, ...) |

Rules

- Declare the command in `game/features/features.hpp` (`extern`) so hooks, other
  features and menus can use it.
- Bind the menu by command name — `sub->add_option<bool_option<>>("inf_stamina"_hash)`
  — and never duplicate the label/description: they come from the command.
  Float commands are bound by pointer because Astra's `number_option<float>`
  only accepts a `number_command<float>` for id binding.
- Persistence is automatic: `command` derives from `state_serializer`, so
  `commands` is one component of `settings.json`.
- Add a new feature by dropping one such file in the right group folder and
  adding one `extern` line; nothing else has to change.
