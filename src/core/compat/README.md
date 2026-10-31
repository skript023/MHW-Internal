# `compat`

Include shims for the pinned Astra host only. `astra/host/canvas.hpp` and
`astra/host/menu_config.hpp` include `"class/vector.hpp"` and `"common.hpp"`, so
this directory is added to the include path to keep the upstream include style
working after the core move. Never add framework code here.
