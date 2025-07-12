#pragma once
#include <vector>
#include "Vec2f.h"
#include "Tile.h"

// Convert patrol points to coarse tile indices
std::vector<Tile> convertPatrolPointsToTiles(const std::vector<Vec2f>& circuit, int tileSize, int enemySize, int colsCoarse, int rowsCoarse);
