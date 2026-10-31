#pragma once

namespace game
{
	// Entry point of the game layer.
	//
	// Creates the MHW pointer table and byte patches, installs every game detour
	// and mid-function hook, wires the Astra menus and registers the tick
	// scripts. Nothing in src/core knows that Monster Hunter: World exists.
	void initialize();

	// Toggles the hooks the framework does not own (ellohim mid hooks).
	void enable();
	void disable();

	// Releases game-owned state. Call after the framework disabled its hooks.
	void shutdown();
}
