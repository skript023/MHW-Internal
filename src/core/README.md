# `core`

Engine-agnostic and game-agnostic. Nothing in this directory may include
`game/`, and no symbol here may read a game pointer table or game setting.

- `common.hpp` — PCH, build identity (`big::config`), process globals
- `memory/` — `big::memory::{handle, range, module, pattern, pattern_batch, pattern_cache}`
- `hooking/` — detours, VMT hooks, byte patching, `hook_manager`, framework presentation hooks
- `logging/` — logger, exception handler, stack traces
- `threading/` — fiber pool, thread pool, coroutine scripts, `script_mgr`
- `config/` — file and folder path management (`file_manager`)
- `commands/` — command objects and the command registry
- `ui/` — ImGui renderer, DX11/DX12 detection + dummy swapchain (`graphic/`),
  Astra menu host, fonts, shared menu base types
- `net/` — websocket/auth client (optional, started with `PRODUCTION`)
- `utility/` — math types, hashing, HWID, subprocess, containers
