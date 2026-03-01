# Grade-Heist

A 2D top-down stealth game built with **C++** and **SFML**. Sneak through a university building, complete 10 mini-game tasks to change your grades, and escape — all while evading a patrolling guard with AI-powered pathfinding.

## Gameplay

- Navigate a tile-based campus map as the player character
- Avoid the enemy guard who patrols a fixed circuit and chases you when you're completing tasks
- Complete **10 unique mini-game tasks** scattered across the map to win
- If the guard catches you (within range with clear line-of-sight), it's game over
- Complete all tasks and reach the exit zone to win

## Features

- **Tile-based world** — 2700×2580 pixel map loaded from Tiled JSON format with a 12px tile grid
- **Enemy AI** — BFS pathfinding on a coarsened grid, with patrol and chase behaviors
- **10 interactive task overlays**, each with unique mechanics:
  - **Code Challenge** — Fill in missing code in a snippet
  - **WiFi Server Shutdown** — Log in and disable the WiFi server
  - **WiFi Credential Crack** — Try random credentials to log into WiFi
  - **Speed Typing** — Type a randomly generated string
  - **Photocopier** — Drag pages into the copier and wait for printing
  - **Computer Terminal** — Solve math and command-line challenges
  - **Integer Overflow** — Input a number that triggers a 32-bit overflow
  - **Combination Lock** — Enter a 4-digit code to crack a safe
  - **PC Simulator** — Flip a switch to enable/disable a PC
  - **Grade Changer** — Log in and change a student's grade
- **Minimap** — Toggleable HUD minimap showing player and enemy positions
- **Lighting effects** — Enemy light-fan visual effect
- **Loading screen** — Async map loading with background music
- **Collision system** — AABB-based wall collision detection

## Project Structure

```
Grade-Heist/
├── src/
│   ├── main.cpp                    # Game entry point and main loop
│   ├── core/
│   │   ├── Constants.h             # World dimensions, speeds, type aliases
│   │   ├── Collision.h / .cpp      # AABB collision detection
│   ├── entities/
│   │   ├── Player.h / .cpp         # Player movement, input, rendering
│   │   └── Enemy.h / .cpp          # Enemy patrol AI, chase, pathfinding
│   ├── world/
│   │   ├── MapLoader.hpp / .cpp    # Tiled JSON map loader (tileson)
│   │   ├── CollisionWalls.h / .cpp # Extract wall rectangles from map
│   │   ├── WalkableGrid.h / .cpp   # Fine-grain walkability grid (12px)
│   │   ├── CoarseGrid.h / .cpp     # Coarsened grid for enemy pathfinding
│   │   ├── Pathfinding.h / .cpp    # BFS pathfinding algorithm
│   │   ├── PatrolCircuit.h         # Hardcoded patrol waypoints
│   │   └── PatrolTiles.h / .cpp    # Waypoint-to-tile conversion
│   ├── ui/
│   │   ├── UI.h / .cpp             # Scene rendering, lighting effects
│   │   ├── Minimap.h / .cpp        # Toggleable HUD minimap
│   │   └── isend.h / .cpp          # Game-over condition detection
│   └── tasks/
│       ├── codechallenge.h / .cpp
│       ├── WiFiServerTask.hpp / .cpp
│       ├── WiFiLoginTask.hpp / .cpp
│       ├── TypingChallenge.hpp / .cpp
│       ├── PhotocopierTask.hpp / .cpp
│       ├── ComputerTaskOverlay.hpp / .cpp
│       ├── ACServerOverlay.hpp / .cpp
│       ├── IntegerOverflowOverlay.hpp / .cpp
│       ├── CombinationLockOverlay.hpp / .cpp
│       ├── PCSimulatorOverlay.hpp / .cpp
│       └── GradeChangerOverlay.hpp / .cpp
├── assets/
│   ├── eita.json / eita.png        # Tiled map data and tileset
│   ├── sprite.png                  # Player sprite sheet
│   ├── buttonimg.png               # Task interaction button
│   ├── gameover.png                # Game-over screen
│   ├── loading.png                 # Loading screen
│   ├── arial.ttf                   # UI font
│   ├── loading_music.ogg           # Loading screen music
│   └── gameplay_music.ogg          # In-game background music
├── CMakeLists.txt                  # Build configuration
├── .gitignore
└── README.md
```

## Dependencies

- **C++17** or later
- **[SFML](https://www.sfml-dev.org/) 2.5+** — Graphics, window, audio, and system modules
- **[Tileson](https://github.com/SSBMTonern/tileson)** — Header-only Tiled JSON map parser (included via `tileson/include/tileson.hpp`)
- **CMake 3.16+** — Build system

## Building

### Linux

```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get install libsfml-dev cmake g++

# Build
mkdir build && cd build
cmake ..
make

# Run (from the project root directory)
cd ..
./build/GradeHeist
```

### macOS

```bash
# Install dependencies
brew install sfml cmake

# Build
mkdir build && cd build
cmake ..
make

# Run (from the project root directory)
cd ..
./build/GradeHeist
```

### Windows (Visual Studio)

```bash
# With vcpkg
vcpkg install sfml

mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake
cmake --build .
```

> **Note:** Run the executable from the project root directory so that asset paths (`assets/...`) resolve correctly.

## Controls

| Key           | Action             |
|---------------|--------------------|
| `W` / `↑`     | Move up            |
| `A` / `←`     | Move left          |
| `S` / `↓`     | Move down          |
| `D` / `→`     | Move right         |
| Mouse click   | Interact with tasks and UI |

## Architecture

The game follows a modular architecture with clear separation of concerns:

- **Core** — Shared constants, types, and collision utilities
- **Entities** — Player and enemy game objects with independent update logic
- **World** — Map loading, grid construction, pathfinding, and patrol route management
- **UI** — HUD elements (minimap, score display), visual effects, and game-over detection
- **Tasks** — Self-contained mini-game overlays, each following a consistent interface (`start()`, `handleEvent()`, `update()`, `draw()`, `isActive()`, `isFinished()`)

## License

This project is for educational purposes.
