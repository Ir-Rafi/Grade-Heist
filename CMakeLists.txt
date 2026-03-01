cmake_minimum_required(VERSION 3.16)
project(GradeHeist LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

# Source files
set(SOURCES
    src/main.cpp

    # Core
    src/core/Collision.cpp

    # Entities
    src/entities/Player.cpp
    src/entities/Enemy.cpp

    # World
    src/world/MapLoader.cpp
    src/world/CollisionWalls.cpp
    src/world/WalkableGrid.cpp
    src/world/CoarseGrid.cpp
    src/world/PatrolTiles.cpp
    src/world/Pathfinding.cpp

    # UI
    src/ui/UI.cpp
    src/ui/Minimap.cpp
    src/ui/isend.cpp

    # Tasks
    src/tasks/codechallenge.cpp
    src/tasks/WiFiServerTask.cpp
    src/tasks/WiFiLoginTask.cpp
    src/tasks/TypingChallenge.cpp
    src/tasks/PhotocopierTask.cpp
    src/tasks/ComputerTaskOverlay.cpp
    src/tasks/ACServerOverlay.cpp
    src/tasks/IntegerOverflowOverlay.cpp
    src/tasks/CombinationLockOverlay.cpp
    src/tasks/PCSimulatorOverlay.cpp
    src/tasks/GradeChangerOverlay.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)
