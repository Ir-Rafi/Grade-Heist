#pragma once

#include <vector>
#include <functional>
#include <iostream>
#include <thread>
#include <atomic>

#include "Constants.h"             // WORLD_W, WORLD_H, TILE_SIZE, Vec2f, WAYPOINT_REACH_THRESHOLD
#include "Collision.h"             // checkCollision(...)
#include "Enemy.h"                 // Enemy(walls, grid, circuit)
#include "Player.h"                // Player class
#include "Minimap.h"               // Minimap HUD
#include "MapLoader.hpp"           // Tiled map loader (with getTaskRects)
#include "codechallenge.h"         // CodeChallenge overlay
#include "WiFiServerTask.hpp"      // WiFiServerTask overlay (wifioff)
#include "WiFiLoginTask.hpp"       // WiFiLoginTask overlay (wifilin)
#include "TypingChallenge.hpp"     // TypingChallenge overlay
#include "PhotocopierTask.hpp"     // PhotocopierTask overlay
#include "ComputerTaskOverlay.hpp" // Computer terminal overlay
#include"ACServerOverlay.hpp"
#include "IntegerOverflowOverlay.hpp"
#include "CombinationLockOverlay.hpp"
#include "PCSimulatorOverlay.hpp"
#include "GradeChangerOverlay.hpp"
#include "UI.h"
#include "isend.h"

// Convert patrol points to coarse tile indices
std::vector<Tile> convertPatrolPointsToTiles(const std::vector<Vec2f>& circuit, int TILE_SIZE, int ENEMY_SIZE, int colsCoarse, int rowsCoarse);
