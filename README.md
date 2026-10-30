# Monster Hunter: World — Internal Mod & Trainer

An internal C++23 DLL trainer and HUD overlay for **Monster Hunter: World** (DirectX 11 / DirectX 12, 64-bit). Built on top of MT Framework reverse engineering, ImGui / Astra UI, and `Ellohim-Hook`.

---

## Overview

- **Target Game:** Monster Hunter: World (Steam / PC x64)
- **Engine:** Capcom MT Framework
- **Binary Type:** Internal Dynamic Link Library (`MHW.dll`)
- **Language Standard:** C++23 (`-std:c++latest`)
- **Hook Engine:** `Ellohim-Hook` (Trampoline, MinHook/HDE mid-function hooks, VMT hooks, hardware breakpoints)
- **Menu System:** Astra UI / Dear ImGui with custom overlays
- **Design Pattern:** Meyer's Singleton architecture across all core service modules

---

## Features

### 1. Visual & HUD Overlays
- **Monster Health & Crown Indicator:** Displays target monster current/max HP, exact size percentage, and crown badges (Miniature 👑, Silver 🥈, Giant Gold 👑).
- **Enrage / Agitator Timer:** Secondary countdown bar for monster enrage state to optimize Clutch Claw flinch shots.
- **Part Health & Severable Tail Cut:** Individual HP bars for monster parts (Head, Torso, Forelegs, Hindlegs, Wings) and remaining severable tail cut threshold with `[CUT]` status.
- **Status Buildup (Ailments Tracking):** Live accumulation bars for Poison, Paralysis, Sleep, Blast, Stun / KO, and Exhaust.
- **Party Damage Meter:** Real-time DPS tracker, total hunt duration, individual damage contribution, hunt percentage share, and cart count.
- **Dropped Materials ESP:** 3D distance tracker, compass bearing (N, NE, E, SE, S, SW, W, NW), and 180s despawn countdown for monster shinies and Guiding Lands drops.
- **Hunter Buff & Mantle Timers:** Active duration bars for Demondrug, Armorskin, Might Seed, Demon Powder, Dash Juice, Cool/Hot Drinks, and Specialized Tool active/cooldown timers.
- **Hunter Coordinates & World Phase:** Real-time 3D coordinates, distance to waypoint pin, area weather, and day/dusk/night phase.

### 2. Combat & Weapon Mechanics
- **Universal Combat:**
  - Invulnerability (Godmode)
  - Infinite Stamina
  - Part Breaker & Easy Part Breaker (1-hit part breaks)
  - Infinite Mounting & Mount Reset
  - Infinite Consumables & Item Use
- **Weapon-Specific Buffs:**
  - **Great Sword:** Instant charge to max charge level
  - **Long Sword:** Infinite Spirit Gauge & Red Level lock
  - **Dual Blades:** Infinite Demon Mode without stamina drain
  - **Charge Blade:** Infinite phials and charged shield
  - **Switch Axe:** Infinite sword gauge
  - **Insect Glaive:** Infinite triple extract buffs (Red + White + Orange)
  - **Bow:** Maximum charge level shots
  - **Heavy / Light Bowgun:** Infinite Wyvernheart ammo, rapid-fire Wyvernheart, no recoil & instant mod application
  - **General:** Infinite sharpness, permanent purple sharpness gauge, infinite slinger ammo, permanent Frostcraft bonus

### 3. World & Quality of Life
- **Minimap & Tracking:** Reveal all monsters permanently on the map, max scoutfly tracking level.
- **Gathering:** Infinite gathering on mining outcrops, bonepiles, and plants.
- **Siege Weapons:** Cannons and ballistas permanently loaded with infinite ammo.
- **Palico:** No cooldown on Palico gadgets, multiplier for Palico experience.
- **Pet Collection:** Instant unlock for all endemic life pets.
- **Vouchers:** Infinite Lucky Vouchers.
- **Rank & Economy:** Configurable multipliers for HR/MR experience, monster research points, and one-click Zenny / Research Points injection.

### 4. Teleportation
- **Waypoint Teleport:** Instantly teleport to active map pin.
- **Forward Teleport:** Teleport directly forward along player view axis.
- **Custom Location Manager:** Save and load custom named coordinates stored in `%APPDATA%/MonsterHunterWorld/Teleport.json`.

### 5. In-Game Equipment & Meal Editors
- **Weapon Editor:** Live modification of equipped weapon attack, defense, affinity, sharpness quality/amount, rarity, and elemental/skill attributes.
- **Armour Editor:** Live modification of armor defense, gem slot levels (slots 1–3 up to Level 4), and elemental resistances (Fire, Water, Thunder, Ice, Dragon).
- **Meal Editor:** Edit active Canteen food skills (Skills A, B, C) and maximize meal stats (+50 HP / +50 Stamina).
- **Crafting Bypass:** Forge and augment equipment without consuming materials or Zenny, automatic max attributes/slots on crafted gear.

---

## Architecture & Code Organization

```
src/
├── commands/           # Looped, bool, int, float, and vector commands
├── features/           # Game mechanic implementations (player, weapon)
├── hooks/              # Native engine function detours and ASM shellcode
├── memory/             # Pattern scanning, module handle wrappers, memory ranges
├── menu/               # ImGui & Astra UI menu pages, submenus, and overlays
├── services/           # Meyer's Singleton services
│   ├── character/      # Hunter state, stats, buffs, coordinates, mantles
│   ├── monster/        # Monster world entities, HP, parts, ailments
│   ├── damage/         # Party damage accounting, DPS, hunt timer
│   ├── drop/           # Dropped shinies, despawn timers, distance tracking
│   ├── equipment/      # Weapon and armour memory structures
│   ├── meal/           # Canteen food stats and skill definitions
│   ├── world/          # Weather and day/night cycle state
│   └── notification/   # In-game notification queue
└── main.cpp            # DLL entry point, thread initialization, lifecycle
```

All services implement the **Meyer's Singleton** design:
- Thread-safe lazy initialization via internal `static &get()`.
- Non-static member functions are `private` with the `_impl` suffix.
- Public interface exposes clean `static` methods without `_impl` (e.g. `character_service::get_position()`, `monster_service::get_health(index)`).

---

## Building from Source

### Prerequisites
- **OS:** Windows 10 / 11 (x64)
- **Compiler:** Visual Studio 2022 or 2026 with *Desktop development with C++* (MSVC v143 / v145+)
- **Build Tools:** CMake 3.20+ and Ninja
- **Windows SDK:** 10.0.19041.0 or newer

### Build Steps

1. Open **x64 Native Tools Command Prompt for VS**:
   ```cmd
   cd /d "L:\Coding\Monster Hunter World"
   ```

2. Configure CMake with Ninja:
   ```cmd
   cmake -B out/build/x64-Release -G Ninja -DCMAKE_BUILD_TYPE=Release
   ```

3. Build the DLL:
   ```cmd
   cmake --build out/build/x64-Release
   ```

4. The compiled binary will be placed at:
   ```
   out/build/x64-Release/MHW.dll
   ```

---

## Usage

1. Launch **Monster Hunter: World**.
2. Inject `MHW.dll` into `MonsterHunterWorld.exe` using any standard 64-bit injector (or the included injector project).
3. Press **`Insert`** to open/close the trainer menu.
4. Settings and custom teleport coordinates are automatically saved to:
   ```
   %APPDATA%\MonsterHunterWorld\settings.json
   %APPDATA%\MonsterHunterWorld\Teleport.json
   ```

---

## Disclaimer

This software is strictly for offline, single-player, and private research / educational purposes. Do not use this tool in public matchmaking sessions or to disrupt the experience of other players.
