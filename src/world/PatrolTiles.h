#pragma once

#include <vector>
#include <utility>
#include "core/Constants.h"
#include "world/Pathfinding.h"

// Convert patrol points to coarse tile indices
std::vector<Tile> convertPatrolPointsToTiles(const std::vector<Vec2f>& circuit, int TILE_SIZE, int ENEMY_SIZE, int colsCoarse, int rowsCoarse);
