#include "CoarseGrid.h"
#include <algorithm>

std::vector<bool> buildCoarseGrid(const std::vector<bool>& grid, int worldW, int worldH, int tileSize, int enemySize) {
    int pp = enemySize / tileSize;
    int cols = worldW / tileSize;
    int rows = worldH / tileSize;
    int colsCoarse = cols - (pp - 1);
    int rowsCoarse = rows - (pp - 1);

    std::vector<bool> coarseGrid(colsCoarse * rowsCoarse, true);
    for (int cy = 0; cy < rowsCoarse; ++cy) {
        for (int cx = 0; cx < colsCoarse; ++cx) {
            bool free = true;
            for (int dy = 0; dy < pp && free; ++dy) {
                for (int dx = 0; dx < pp; ++dx) {
                    int ry = cy + dy;
                    int rx = cx + dx;
                    if (grid[ry * cols + rx]) {
                        free = false;
                        break;
                    }
                }
            }
            coarseGrid[cy * colsCoarse + cx] = free;
        }
    }
    return coarseGrid;
}
