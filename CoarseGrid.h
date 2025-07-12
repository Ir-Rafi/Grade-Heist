#pragma once
#include <vector>

// Build coarse grid for enemy pathfinding
std::vector<bool> buildCoarseGrid(const std::vector<bool>& grid, int worldW, int worldH, int tileSize, int enemySize);
