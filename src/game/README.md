# `game`

Everything Monster Hunter: World specific. This is the only layer allowed to
know about the game's pointer table, offsets, settings and menus.

- `game.cpp` — `game::initialize()` / `game::enable()` / `game::disable()` /
  `game::shutdown()`, the single entry point the app calls
- `pointers.{hpp,cpp}` — pattern-scanned pointer table
- `patches.{hpp,cpp}` — game byte patches (`g_patches`)
- `settings.hpp` — trainer settings (`g_settings`), persisted as JSON
- `classes/` — in-memory game classes (`fwddec.hpp`) and math types shared with the game ABI
- `hooks/` — game detours, ellohim mid hooks, shellcode stubs and the registration list
  (`install.cpp`, `hooks.hpp`)
- `features/` — player/weapon feature logic and the command objects bound to settings
- `menu/` — Astra submenus, overlay and notification widgets
- `mhw/` — game services (camera, character, damage, drop, equipment, meal,
  monster, notification, party, qol, world)
- `events/` — tick scripts bound to features and services
