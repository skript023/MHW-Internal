# `engine`

Reserved for engine-level abstractions (the equivalent of `engine/mono` and
`engine/unity` in the reusable framework).

Monster Hunter: World exposes no managed runtime, so this layer is currently
empty: the detours, mid hooks and byte patches that talk to the game engine all
live in `src/game`, which is allowed to depend on this directory once an
abstraction is introduced.

Layer contract when something lands here: `engine` may include `core` only, and
`game` may include `engine` and `core` — never the other way around.
