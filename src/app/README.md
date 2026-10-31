# `app`

The DLL entry point only. `main.cpp` waits for the target window, brings up the
framework (file manager, logger, renderer, pools, `hook_manager`), calls
`game::initialize()`, then runs the main loop. No game-specific code belongs
here: if a step knows about MHW offsets or menus, it belongs in `src/game`.
