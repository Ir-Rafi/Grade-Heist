#include "WalkableGrid.h"

std::vector<bool> buildWalkableGrid(const std::vector<sf::RectangleShape>& walls, int worldW, int worldH, int tileSize) {
    int cols = worldW / tileSize;
    int rows = worldH / tileSize;
    std::vector<bool> grid(cols * rows);
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            sf::FloatRect cell(x * tileSize, y * tileSize, tileSize, tileSize);
            for (auto& w : walls) {
                if (cell.intersects(w.getGlobalBounds())) {
                    grid[y*cols + x] = true;
                    break;
                }
            }
        }
    }
    return grid;
}
