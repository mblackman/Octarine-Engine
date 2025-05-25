# Octarine-Engine

Octarine Engine colors imagination as a lightweight 2D game engine that is present where magic happens. It has its own ECS, supports Lua scripting, and is a yellowish-purple color.

This engine utilizes:

- GLM: <https://github.com/g-truc/glm>
- ImGui: <https://github.com/ocornut/imgui>
- Lua5.3: <https://www.lua.org/>
- SDL2: <https://www.libsdl.org/>
- spdlog: <https://github.com/gabime/spdlog>

## Ubuntu Setup

Run this command to install the necessary library and tools:

```sh
sudo apt install build-essential libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev liblua5.3-dev lua5.3 libspdlog-dev
```

## Run Instructions

Build the engine and the game with:

```sh
make
```

Start the game with:

```sh
make run
```
