#include "PatrolTiles.h"
#include <algorithm>

std::vector<Tile> convertPatrolPointsToTiles(const std::vector<Vec2f>& circuit, int tileSize, int enemySize, int colsCoarse, int rowsCoarse) {
    int pp = enemySize / tileSize;
    std::vector<Tile> patrolTiles;
    for (auto& pt : circuit) {
        int cx = int(pt.x / tileSize) - (pp / 2);
        int cy = int(pt.y / tileSize) - (pp / 2);
        cx = std::clamp(cx, 0, colsCoarse - 1);
        cy = std::clamp(cy, 0, rowsCoarse - 1);
        patrolTiles.emplace_back(cx, cy);
    }
    return patrolTiles;
}
